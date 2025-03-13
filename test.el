;; Load the compiled module
(module-load "/home/jeff/code/emacs-ffi-to-cpp/lib/example.ln-1.so")

;; Call the function
(message "Result: %d" (add-one 41))
(message "Reversed: %s" (reverse-second "ignored" "hello"))
