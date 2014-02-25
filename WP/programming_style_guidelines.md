

Programming Style Guidelines
----------------------------

The programming style used in the laboratory works somehow coincides with the one described in [this link](http://www.physics.ox.ac.uk/users/iontrap/ams/cpp_help/mycppstyle.htm), however there are some personal preferences that will be presented below as a part of a modified version of the style from the link.

#### Naming conventions

Overall the code is written in CamelCase without the use of Hungarian notation except for pointers (p/lp) and some Windows specific types like handles plus the names of the parameters for standard windows function prototypes (WinMain, WinProc, etc..) for example `wParam` and `lParam`.

Names should be meaningfull but not too long, short names are alowed for variables with small scope like counters in a loop, or for things like a `PAINTSTRUCT ps;` that will be used to fill the structure and send it in `BeginPaint( ... )` in the next couple of lines.

* `globals` should be prefixed with 'g_' and must be used with caution (preferably they should not exceed the file scope)

* `static` function variables should be prefixed with 's_'

* `class members`, static and non-static, should NOT be prefixed with 'm_' or 's_'

* `variable` names - camelCase starting with a lowercase letter

* `function` and _class_ names - CamelCase starting with a capital letter

* `#define`'s - SNAKE_CASE, all caps



#### Indentation and braces

* Indent with tabs of size 4

* ALWAYS use braces in cases like `if`'s with one statement

* For `if`'s with one short statement it is possible to use the following:
```
if (condition) { SomeFunction(); }
```

* Use 'Allman' indentation and braces style:
```
if (condition)
{
	// code
}
```


#### Other

* ~~goto~~

* Use prefix increment/decrement (`++i`) operators instead of postfix (`i++`), except for special cases

