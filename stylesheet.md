# Coding Style #

This is the reference sheet for how the code in the repo should be styled. 
It is currently subject to update. Although, mainly in the form of increased detail.

When in doubt, refer to McConnell chapter 31.

&nbsp;

### Control Layout ###
Emulating Pure Blocks

Example:

```
while ( pixelColor == Color_Red ) {
    statement1;
    statement2;
}
```

&nbsp;

One "exception" to the book is switch statements. The reason for this is that "break;" is a statement, not an "end" block.

Example:

```
switch ( pixelColor ) {
    case Color_Red:
        statement1;
        statement2;
        break; //Same indentation as the statements
    default:
        statement1;
        statement2;
        break; //This should still be here for consistency
}
```

&nbsp;

Single-statement blocks should be enclosed with braces. (For maintainability)

Example:

```
if ( expression ) {
    statement1;
}
```

&nbsp;

Goto formatting should be the same as the book. (See pages 750-751)

&nbsp;

### Individual statements ###
Use spaces around array refrences.

Example:

```
grossrate[ census[ groupId ].gender, census[ groupId ].ageGroup ];
```

&nbsp;

Use spaces around function arguments.

Example:

```
GetCensus( inputFile, empCount, empData, maxEmps, inputError );
```

&nbsp;

For continuation lines, follow the book. (See pages 754-758)

Use only one statement per line.

Likewise, put each data declaration on its own line.

Put the asterisk for pointers against the variable.

Example:

```
File *inputFile;
```


Comments should be the same indentation as their surrounding statements. They should (in general) be preceded by a blank line.
The only "exception" is commented out code.

&nbsp;

### Functions/Classes ###
One argument per line in the header of a function, for large argument functions. 

Example:

```
public void InsertionSort(
    SortArray data,
    int firstElement,
    int lastElement
)
```

&nbsp;

For a small amount write the arguments on the same line.

Example:

```
public void InsertionSort( SortArray data, int firstElement, int lastElement )
```

&nbsp;

Class Layout see book on page 768.

No two classes should be in the same file. (There might be reasonable exceptions, but by default DO NOT DO IT)

Use two blank lines between functions.

Source File Ordering (see page 773)
