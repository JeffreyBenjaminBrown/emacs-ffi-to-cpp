#include <emacs-module.h>
#include <string>
#include <algorithm>

int plugin_is_GPL_compatible;

static int add_one(int num) {
  return num + 1; }

static std::string reverse_second_string
  (const std::string& first, const std::string& second) {
    std::string result = second;
    std::reverse(result.begin(), result.end());
    return result; }

static emacs_value emacs_add_one
  ( emacs_env *env,
    ptrdiff_t nargs,
    emacs_value args[],
    void *data) noexcept {

    intmax_t num = // Extract the integer argument
      env->extract_integer(env, args[0]);
    return env->make_integer // Return an Emacs integer
      ( env,
	add_one(static_cast<int>(num))); }

// Return the second argument (a string) reversed
static emacs_value emacs_reverse_second
  ( emacs_env *env,
    ptrdiff_t nargs,
    emacs_value args[],
    void *data) noexcept {

    // Get the size of the second string
    ptrdiff_t size;
    env->copy_string_contents(env, args[1], NULL, &size);

    // Make a buffer for it
    std::string second_str(size, '\0');
    env->copy_string_contents(env, args[1], &second_str[0], &size);

    // Reverse it
    std::string result =
      reverse_second_string("ignored", second_str);

    // Return the reversed string as an Emacs value
    return env->make_string
      ( env,
        result.c_str(),
        result.length()); }

extern "C" int emacs_module_init
  (struct emacs_runtime *ert) noexcept {

    emacs_env *env = ert->get_environment(ert);

    // Make and register the add-one function
    emacs_value func = env->make_function
      (env, 1, 1, emacs_add_one,
       "Add 1 to the given number.", NULL);
    emacs_value symbol = env->intern(env, "add-one");

    // Make an array for arguments
    emacs_value args[2] = {symbol, func};
    env->funcall(env, env->intern(env, "defalias"), 2, args);

    // Make and register the reverse-second function
    emacs_value func2 = env->make_function
      (env, 2, 2, emacs_reverse_second,
       "Reverse second string, ignore first.", NULL);
    emacs_value symbol2 = env->intern(env, "reverse-second");

    // Make an array for arguments
    emacs_value args2[2] = {symbol2, func2};
    env->funcall(env, env->intern(env, "defalias"), 2, args2);

    // Have Emacs say so when the module loads
    emacs_value msg = env->make_string(env, "C++ module loaded successfully", 29);
    emacs_value msg_args[1] = {msg};
    env->funcall(env, env->intern(env, "message"), 1, msg_args);

    return 0;
}
