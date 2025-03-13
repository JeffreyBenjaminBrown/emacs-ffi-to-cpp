# USAGE

Run `compile.sh`, then open and evaluate `test.el` in Emacs.


## PITFALL: Reloading external modules in Emacs sucks

At the top of `test.el` there is the following line:
```
(module-load "/home/jeff/code/emacs-ffi-to-cpp/ln.so")
```

Once that has been evaluated, reevaluating it has no effect,
because Emacs caches the result.
The workaround I have been using
is to use a series of symbolic links to the relevant .so file:
ln.so, l1.so, l2.so ...
Thus each recompilation step involves three steps:

  * recompiling
  * dcreating a new symlink
  * editing `test.el` to load the new symlink
