~~ well, hello there!
~~ U r now reading quick start guide for ReiLang, small and easy scripting language
~~ btw, it's single-line comment
~~[
    and it's multiline one
]~~~~[
    in ReiLeng there are two basic numeric types: Int and Real.
    var and con defenitions are pretty simple:
]~~
var x = 5; ~~ this line will create a variable with name 'x' and current type Int
x = 5.5;   ~~ now 'x' is Real.
~~[
    we can represent any basic var using 5 fields:
        1) name
        2) c/v qualifier
        3) lock/unlock type. if type is locked, we cant change it during runtime.
        4) tipe itself
        5) value
]~~
~~ here are some examples:
var y :: Int;                       ~~ create locked Int variable with some standart value (0 for Int)
y = 5;                              ~~ ok
~~ y = 5.5;                         ~~ error, because 5.5 is Real;
                                    ~~ ofc, we can declare and define vars/cons in single line, moreover, we can define not one, 
                                    ~~ but infinite number of vars/cons, separated by commas 
var z :: Int = 5, a :: Real, b = 0; ~~ btw, semi is required after any statement
con PI = 3.14, TAU = 2 * PI;        ~~ here are some constants
~~[
    now some words about type casting.
    if casting leads to widening conversion, it will be done implicitly, otherwise it must be done explisitly.
    some examples:
]~~
var real_var :: Real = 5;    ~~ ok
~~ var int_var :: Int = 5.5; ~~ error
var any_var;
any_var = 5;                 ~~ ok
any_var = 5.5;               ~~ also ok

~~[
    now lets talk about arithmetic:
    (i will use haskell style)
        (+)  :: a -> b -> widest a b
        (-)  :: a -> b -> widest a b
        (*)  :: a -> b -> widest a b
        (/)  :: a -> b -> Real
        (//) :: a -> b -> Int
        (%)  :: a -> b -> widest a b
]~~
~~ var s :: Int = 4 / 2; ~~ error
var t :: Int = 4.5 // 2; ~~ ok
~~[
    in ReiLang {} are used to represent the block of code:
]~~
var test = 5;
{
    test = test + 1; ~~ ok
    var inner = 0;
}
inner = 1; ~~ also ok
