## 7/4/2016
 . Initial version
## 7/10/2016
 . Add more types, functions, and object support
## 7/31/2016
 . Improve scope lookup
 . Add width, throw, while, ...
 . Improve regression tests and coverage
## 8/7/2016
 . Improve chained scope lookup
 . Improve regression tests and coverage
 . Code cleanup, separate files per class
 . More canvas function support
## 8/14/2016
 . split type functions into type and object versions
 . rename base object class and add Object class
 . Add more base type functions
 . Add this keyword
 . support index by name of this to create property
 . single instance of false, true, undefined and null types
 . add infinity and nan values
 . Improve error message line numbers.
# 8/28/2016
 . Support 2d and higher array assign
 . Add break and continue label names
 . Add labels
 . Fix expression multiple operator unstack on precedence
 . Improve token line number tracking
 . Add do .. while
 . Add require to load .js file
 . Add RegExp support
 . Add expression evaluation debug
 . Add requestAnimationFrame
# 9/5/2016
 . Add error objects
 . support throw and catch with try
 . Improve index/iteration support
 . Improve math and expression handling of NaN and Inf values
 . Remove std::move (not sure where they appeared from)
 . Better prototype definition and variable/function support
 . Support unbalanced [] is complete line check
 . Add startup files for and fast start mode
# 9/11/2016
 . More throw handling
 . Fix switch with no break
 . Improve support for functions defined and returned from functions
 . Supprt comma separated args in Function constructor
 . Improve 'in' operator support
 . Improve 'void' operator support
 . Add JSON class
 . Support UTF* encoding/decoding for strings
# 9/19/2016
 . Improve prototype handling
 . Make function object it's own type
 . Improve delete operator support
 . Separate type and object function invocation methids
 . Add separate boolean object
 . support object/non-object strings and numbers
 . new operator uses prototype object
 . make var variables undeletable
 . improve property lookup for lvalue to include more parent data and calculaet value on demand
 . support function direct call on declaration

# 9/25/2016
 . Add enumerable, deletable for properties
 . Create object for {} not dictionary
 . Add statement for semi-colon separated tokens
 . Add escape, encodeURI, encodeURIComponent, unescape, decodeURI, decodeURIComponent global funcs
 . Support indent in json stringify
 . Support base in parseInt
 . Add Object.defineProperties
 . Improve UTF8 encoding/decoding support for strings
 . Use fpclassiify for NaN, Infinity checks

# 10/1/2016
 . Pass type to object and type functions
 . Add more array functions
 . split print and toString implementations
 . add apply and bind function methods
 . add frozen, sealed and extensible APIs - not supported yet
 . add array-like object support for objects
 . inherit prototype values
 . add regexp flags and match index support

# 10/9/2016
 . Add more generic support to array functions
 . toLocaleString uses named function
 . Use sparse implementaion for array data
 . Take advantage of sparse data in array functions
 . Add use function support for sort
 . Support indexed this variable
 . Use make_shared instead of new calls
