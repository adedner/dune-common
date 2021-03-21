import os
import sys

def add_config_header(config_file, dir):
  if not os.path.exists(dir):
    print('directory {} does not exist.'.format(dir))
    exit(2)

  config_header = '#include <{}>'.format(config_file)
  for root, dirs, files in os.walk(dir):
    for filename in filter(lambda f: f.endswith('.hh'), files):
        file = os.path.join(root,filename)
        print(file)

        contains_config_header = False
        with open(file,'r') as f:
          file_content = [line.rstrip() for line in f.readlines()]
          for l,line in enumerate(file_content):
            if config_header in line:
              contains_config_header = True
              break
            if l >= 50:
              break

        if not contains_config_header and len(file_content) > 0:
          with open(file,'w') as f:
            config_header_written = False
            include_guard_written = 0
            for line in file_content:
              # find the include guard
              if include_guard_written == 0:
                if line.startswith('#ifndef DUNE_'):
                  include_guard_written = 1
                elif line.startswith('#pragma once'):
                  include_guard_written = 2
              elif include_guard_written == 1 and line.startswith('#define DUNE_'):
                 include_guard_written = 2
              elif include_guard_written == 2 and not config_header_written:
                f.write(config_header + '\n')
                config_header_written = True

              f.write(line + '\n')

if __name__ == '__main__':
  if len(sys.argv) < 3:
    print('usage: {} <config_file> <dir>'.format(sys.argv[0]))
    exit(1)

  add_config_header(sys.argv[1], sys.argv[2])
