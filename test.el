;; Load the compiled module
(module-load "/home/jeff/code/emacs-ffi-to-cpp/example.so")

;; Call the function
(message "Result: %d" (cpp-add-one 41))
