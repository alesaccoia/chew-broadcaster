def cmd(cmd):
    import subprocess
    import shlex
    return subprocess.check_output(shlex.split(cmd)).rstrip('\r\n')

def get_template(file):
    with open(file, 'r') as f:
        import string
        return string.Template(f.read())

def get_tag_info(archive, tag):
    rev = cmd('git -C {0} rev-parse {1}'.format(archive, tag))
    anno = cmd('git -C {0} cat-file -p {1}'.format(archive, rev))
    tag_info = []
    for i, v in enumerate(anno.splitlines()):
        if i <= 4:
            continue
        tag_info.append(v.lstrip())

    return tag_info

import shutil
def create_ppa(tag, jenkins_build, distro, version_suffix, update):
    
    cmd('git clone https://github.com/jp9000/obs-studio.git')
    cmd('git -C obs-studio checkout {0}'.format(tag))
    cmd('git -C obs-studio submodule update --init --recursive')
    import re
    version = re.sub(r'(([0-9]|[.])*)-([0-9]*)-.*', r'\1.\3', cmd('git -C obs-studio describe'))
    if version_suffix is not None:
        version += '.' + version_suffix

    archive = 'obs-studio_{0}'.format(version)

    if update:
        shutil.rmtree('obs-studio')
    else:
        shutil.move('obs-studio', archive)

    import os
    debian_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'debian')
    args = {
        'version': version,
        'jenkins_build': jenkins_build,
        'changelog': '  '+'\n  '.join(get_tag_info(archive, tag)),
        'date': cmd('date -R'),
        'distro': distro,
    }
    control_template = get_template(os.path.join(debian_dir, 'changelog'))

    if update:
        shutil.rmtree('{0}/debian'.format(archive))

    shutil.copytree(debian_dir, '{0}/debian'.format(archive))

    with open('{0}/debian/changelog'.format(archive), 'w') as f:
        f.write(control_template.substitute(args))

    if not update:
        cmd('tar cvzf {0}.orig.tar.gz {0}'.format(archive))

    print version

if __name__ == "__main__":

    import argparse
    parser = argparse.ArgumentParser(description='obs-studio ubuntu ppa util')
    parser.add_argument('-j', '--jenkins-build', dest='jenkins_build')
    parser.add_argument('-t', '--tag', dest='tag')
    parser.add_argument('-s', '--suffix', dest='suffix', default=None)
    parser.add_argument('-d', '--distro', dest='distro', default=None)
    parser.add_argument('-u', '--update', dest='update', required=False, action='store_true', default=False)
    args = parser.parse_args()

    create_ppa(args.tag, args.jenkins_build, args.distro, args.suffix, args.update)
