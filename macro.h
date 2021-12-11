#ifndef MACRO_H
#define MACRO_H

#define SINGLETON_FUNC(className) \
  className& get ## className();
#define SINGLETON_FUNC_DEF(className) \
  className& get ## className() { static className instance; return instance; }

#endif // MACRO_H
