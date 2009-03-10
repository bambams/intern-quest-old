=======================================================================

Intern Quest, COSC2036 2009

by Brandon McCaig & Samuel Henderson

Website: <http://code.google.com/p/intern-quest/>


 Tools and Libraries Used
-----------------------------------------------------------------------

    Compiler: GNU GCC, MinGW

    Platform: Linux, Windows


 Dependencies
------------------------------------------------------------------------

    * Allegro (4.2.2): <http://www.allegro.cc/files/>
        - In Linux, check your distribution's software repository.
        - Otherwise, there are binaries available and building in Linux
          shouldn't be too much trouble. Building in Windows is also an
          option, but personally I find the binaries so much simpler (just
          drop the contents of the binary package in your compiler's
          installation directory or somewhere else they will be found; search
          the allegro.cc forums for help).
    * Boost (1.34.1): <http://www.boost.org/users/download/>
        - In Linux, check your distribution's software repository.
        - At the time of writing, everything being used thus far is header
          only so there's no need to build anything. Just drop the header
          files in your compiler's installation.
    * POSIX Threads
        - POSIX compliant systems probably already have it.
        - Windows users will need to get POSIX threads For Win32
          <http://sourceware.org/pthreads-win32/>.
    * TinyXML (2.5.3): <http://www.grinninglizard.com/tinyxml/>
        - In Linux, check your distribution's software repository.
        - Otherwise, it should suffice to drop the source files into the
          project (though you'll need to use Makefile.old to build because it
          isn't planned to add the TinyXML source files to Makefile.bam).


 Notes on Compiling
-----------------------------------------------------------------------

N/A


 About the Game
-----------------------------------------------------------------------

N/A


 How to Play
-----------------------------------------------------------------------

N/A

