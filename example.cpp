#include <emacs-module.h>

int plugin_is_GPL_compatible;

static int add_one(int num) {
  return num + 1; }

static emacs_value emacs_add_one
  ( emacs_env *env,
    ptrdiff_t nargs,
    emacs_value args[],
    void *data) noexcept {

    intmax_t num = // Extract the integer argument
      env->extract_integer(env, args[0]);
    return env->make_integer // Return an Emacs integer
      ( env,
	add_one(static_cast<int>(num));); }

extern "C" int emacs_module_init
  (struct emacs_runtime *ert) noexcept {

    emacs_env *env = ert->get_environment(ert);

    // Create and register the add-one function
    emacs_value func = env->make_function
      (env, 1, 1, emacs_add_one,
       "Add 1 to the given number.", NULL);
    emacs_value symbol = env->intern(env, "cpp-add-one");

    // Create an array for arguments
    emacs_value args[2] = {symbol, func};
    env->funcall(env, env->intern(env, "defalias"), 2, args);

    // Have Emacs say so when the module loads
    emacs_value msg = env->make_string(env, "C++ module loaded successfully", 29);
    emacs_value msg_args[1] = {msg};
    env->funcall(env, env->intern(env, "message"), 1, msg_args);

    return 0;
}
