#include <Windows.h>


namespace GGS {
  namespace Application {

    class WindowHelper
    {
    public:

      /*!
        \fn static void WindowHelper::activate(HWND window);
        \brief Activates the given window.
        \author Ilya.Tkachenko
        \date 20.08.2012
        \param window Handle of the window.
      */
      static void activate(HWND window);
    private:
      WindowHelper();
      ~WindowHelper();
    };

  }
}