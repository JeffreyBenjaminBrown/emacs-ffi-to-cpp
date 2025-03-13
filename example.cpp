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

    ptrdiff_t size; // size of the second string
    env->copy_string_contents(env, args[1], NULL, &size);

    std::string second_str(size - 1, '\0');
    env->copy_string_contents ( env,
			        args[1],
			        &second_str[0], // puts string here
			        &size);

    std::string result =
      reverse_second_string ("ignored", second_str);
    return env->make_string ( env,
			      result.c_str(),
			      result.length() ); }

extern "C" int emacs_module_init
  (struct emacs_runtime *ert) noexcept {

    emacs_env *env = ert->get_environment(ert);

    // Make and register add-one, with an array for its arguments
    emacs_value func = env->make_function
      (env, 1, 1, emacs_add_one,
       "Add 1 to the given number.", NULL);
    emacs_value symbol = env->intern(env, "add-one");
    emacs_value args[2] = {symbol, func};
    env->funcall ( env,
		   env->intern(env, "defalias"),
		   2,
		   args);

    // Make and register reverse-second, with an array for its args.
    // PITFALL: Each function appears to need a separate args array.
    emacs_value func2 = env->make_function
      (env, 2, 2, emacs_reverse_second,
       "Reverse second string, ignore first.", NULL);
    emacs_value symbol2 = env->intern(env, "reverse-second");
    emacs_value args2[2] = {symbol2, func2};
    env->funcall ( env,
		   env->intern(env, "defalias"),
		   2,
		   args2);

    // Have Emacs say so when the module loads
    emacs_value msg = env->make_string
      (env, "C++ module loaded successfully", 29);
    emacs_value msg_args[1] = {msg};
    env->funcall ( env,
		   env->intern(env, "message"),
		   1,
		   msg_args);

    return 0;
}
