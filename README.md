# USAGE

Run `compile.sh`, then open and evaluate `test.el` in Emacs.


## PITFALL: Why ./compile.sh is complex

Reloading external modules in Emacs sucks. If you rerun `module-load` in Emacs, nothing happens, because Emacs caches the result. My workaround is to create a symlink with a number in its name, pointing to the shared object file of interest, and load that each time. This explains some of the files that appear in the `./volatile` folder: One is the load command to run in Emacs, and another is the iteration number. (The third, `where-is-emacs-module-dot-h.txt`, is only necessary because I'm using NixOS.)
