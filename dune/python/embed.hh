#ifndef DUNE_PYTHON_EMBED_HH
#define DUNE_PYTHON_EMBED_HH

#include<dune/python/pybind11/embed.h>
#include<Python.h>

#include<elf.h>
#include<link.h>
#include<string>
#include<filesystem>

namespace Dune {

  namespace impl {

    std::wstring read_rpath_variable(int which) {
      // Extract the R(UN)PATH from the dynamic header section
      const ElfW(Dyn) *dyn = _DYNAMIC;
      const ElfW(Dyn) *rpath = NULL;
      const char *strtab = NULL;
      for (; dyn->d_tag != DT_NULL; ++dyn) {
        if (dyn->d_tag == which) {
          rpath = dyn;
        } else if (dyn->d_tag == DT_STRTAB) {
          strtab = (const char *)dyn->d_un.d_val;
        }
      }
      if (strtab != NULL && rpath != NULL) {
        strtab = strtab + rpath->d_un.d_val;
      }

      // Widen the string
      std::size_t len = strlen(strtab);
      const wchar_t* widened = Py_DecodeLocale(strtab, &len);

      return std::wstring(widened);
    }

  } // namespace impl

  auto get_scoped_interpreter(bool init_signal_handlers = true)
  {
    // Gather a list of potential paths by inspecting both the RPATH
    // and the RUNPATH entry in the dynamic header section
    std::vector<std::wstring> paths;
    std::wstringstream wss(impl::read_rpath_variable(DT_RPATH));
    std::wstring temp;
    while(std::getline(wss, temp, L':'))
        paths.push_back(temp);
    wss = std::wstringstream(impl::read_rpath_variable(DT_RUNPATH));
    while(std::getline(wss, temp, L':'))
        paths.push_back(temp);

    // Iterate over these paths and find Python
    for(auto path : paths)
    {
      std::wstring fullpath = path + std::wstring(L"/python");
      if (std::filesystem::exists(fullpath)) {
        // If we find it, we point the Python initialization procedure to it
        Py_SetProgramName(fullpath.c_str());
        break;
      }
    }

    // The actual initialization of the interpreter is done by
    // pybind11::scoped_interpreter.
    return pybind11::scoped_interpreter(init_signal_handlers);
  }

} // namespace Dune

#endif
