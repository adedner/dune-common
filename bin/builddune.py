import getopt
try:
    from dune.common.module import resolve_dependencies, resolve_order, select_modules, is_installed
    import dune.common
    print(dune.common.__file__)
except ImportError:
    import os
    here = os.path.dirname(os.path.abspath(__file__))
    mods = os.path.join(os.path.dirname(here), "python", "dune", "common")
    if os.path.exists(os.path.join(mods, "module.py")):
        import sys
        sys.path.append(mods)
        from module import resolve_dependencies, resolve_order, select_modules, is_installed
    else:
        raise

def call(module,folder,command):
    print("calling command",command,"for",module,"in",folder)


def main(argv):
    try:
        opts, command = getopt.getopt(argv, "h", ["help", "only=", "module="])
    except getopt.GetoptError:
        print('usage: pythonpackage.py [--only | -module]')
        sys.exit(2)

    only = None
    module = None
    for opt, arg in opts:
        if opt in ('-h',"--help"):
            print('usage: builddune.py [--only | --module]')
            sys.exit(2)
        elif opt in ("--module"):
            module = arg
        elif opt in ("--only"):
            only = arg
    assert (only is None or module is None)

    print(only,module)

    # find all modules with their folders
    modules, dirs = select_modules()
    deps = resolve_dependencies(modules)

    # execute modules in the order they appear
    if module is not None:
        if not module in modules:
            print("module",module,"not found")
            sys.exit(2)
        if is_installed(dirs[module],modules[module]):
            print("module",module,"is installed")
            sys.exit(2)
        moduleDeps = resolve_dependencies(modules,module)
    else:
        moduleDeps = set()
    if only is not None:
        if not only in modules:
            print("module",only,"not found")
            sys.exit(2)
        if is_installed(dirs[only],modules[only]):
            print("module",only,"is installed")
            sys.exit(2)
        call(modules[only],dirs[only],command)
    else:
        for m in resolve_order(deps):
            if not is_installed(dirs[m],modules[m]):
                if module is None or m in moduleDeps:
                    call(modules[m],dirs[m],command)

if __name__ == "__main__":
    main(sys.argv[1:])
