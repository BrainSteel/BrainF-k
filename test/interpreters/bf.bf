[
    bf.bf
    A Brainfuck interpreter written in Brainfuck.
]

[ Initialization ]
->->+>

[ Read program until a dollar sign (36) is found. ]
,>++++++[-<------>]<
[
    >>,
    >++++++[-<------>]
    <
]~

[
    Current state:
    [ 255 255 1 c1 0 c2 0 c3 0 c4 0 ... c_n 0 0 0 0 ... ]
                                              ^
    We will leave this cell blank, and use it as the point of
    termination for the interpreter.

    Note that c1, c2, ..., c_n are the ASCII values of each character
    in the Brainfuck program that is input, minus 36.

    This means we have the following mapping between values
    and Brainfuck commands:
    + = 7
    , = 8   = '+' + 1
    - = 9   = ',' + 1
    . = 10  = '-' + 1
    < = 24  = '.' + 14
    > = 26  = '<' + 2
    [ = 55  = '>' + 29
    ] = 57  = '[' + 2

    We will use the offsets in the above to determine which
    command needs to be executed.

    (We will assume that there is no other character than the 8 BF commands.)
]]

[ Leave the current space at 0 and leave a 255 four spaces to the right. ]
>>>>->

[ We now mark this cell and the following cell as the current cell pointer. ]
+>~

[ Go back to the beginning of the tape and begin executing the Brainfuck code. ]
<+[-<<+]->>>
[
    
    >
    [ Move the instruction to the right exactly one cell to the right to make room. ]
    >[->+<]<<
    
	-------[
	    -[
	        -[
                -[
                    --------------[
                        --[
                            >+++++[-<------>]<+~[
                            	+[->>+]
                            	[ Go to the current cell pointer. Leave an extra 255 to the left. ]
                            	-<->>-[+>>-]

                                [
                                    Leave a marker of 1 to the right. 
                            	    If the cell here is nonzero...
                                ]
                                +>>+<~[
                                    >[-<<+]-<+[-<<+]
                                    [ Go ALL the way back to the beginning of the tape. ]
                                    -

                                    >-[+>>-]~
                                    [ Go to the instruction pointer. ]

                                    [ Reset the value of this instruction back to 57. ]
                                    +++++[->+++++++++++<]

                                    +>>>
                                    [ Move the instruction to the right back to its place. ]
                                    >[-<+>]

                                    [
                                        Use the instruction pointer as a counter for brackets.
                                        While it is nonzero...
                                    ]
                                    ~<<<<[
                                        <<+++++[->-----------<]
                                        
                                        [ If the instruction to the left is not 55 ('[') ... ]]
                                        >[
                                            --[
                                                ++

                                                >[-<<+>>]~
                                            ] [[ End If: instruction to the left is not 57 (']') ]
                                            >>[
                                                <<++

                                                >+[-<<+>>]>>~
                                            ] [[ End If: instruction to the left is 57 (']') ]

                                            <<+++++[-<+++++++++++>]
                                        ]
                                        >>[
                                            <<<+++++[->+++++++++++<]

                                            [
                                                Decrease the bracket counter by 1, move it
                                                to the left, and exit.
                                            ]
                                            >>-[-<<+>>]>>
                                        ]

                                        [ The bracket counter is now four cells to the left. ]
                                        <<<<
                                    ]

                                    [
                                        Set the new instruction pointer at the bracket, then
                                        move the instruction to the right to the right one cell.
                                    ]
                                    +>>>[->+<]

                                    [ Zero the open bracket, since 55 will be added to it later. ]
                                    <<[-]
                                    
                                    [ Find the 255 before the interpreted program cells. ]
                                    +[->>+]-<<<
                                    
                                ] [ End If: cell is nonzero. ]
                                >[
                                    [-<<+]-<
                                    [ Find the 255 before the interpreted program cells. ]
                                ] [ End If: cell is zero. ]
                                [ Remove the first 255 to the right. ]
                                >+<

                                [ Go to the instruction pointer and exit the loop. ]
                                <-[+<<-]+>>>
                                
                            ] [[ End If: c == ']' ]
                            <
                            [
                                >>+++++[-<+++++++++++>]
                                [
                                    Above, we reset the instruction to 55.
                                    Below, we move the instruction to the right back to
                                    its proper place.
                                ]
                                >>[-<+>]

                                [
                                    Use the current instruction pointer as a bracket counter.
                                    While the bracket counter is nonzero...
                                ]
                                <<<<[~
                                    >>+++++[->-----------<]~

                                    [ If the instruction to the right is not 55 ('[') ... ]]
                                    >[
                                        --[
                                            >~ [ Do absolutely nothing. ]
                                        ] [ End If: instruction to the right is not 57. ]
                                        
                                        <<[
                                            <->>~ [ Lower the bracket counter by 1. ]
                                        ] [ End If: instruction to the right is 57. ]

                                        [ Undo the subtraction of 2 and exit the if. ]
                                        >++>~ 
                                    ] [ End If: instruction to the right is not 55. ]

                                    <<[
                                        <+>>~
                                    ] [ End If: instruction to the right is 55. ]

                                    [ Undo the subtraction of 55. ]
                                    +++++[->+++++++++++<]~

                                    [ Copy the bracket counter to the left and repeat. ]
                                    <<[->>+<<]>>
                                ]
                                
                                <<+>~>

                                [
                                    Subtract 55 from the instruction we found and move the
                                    instruction immediately to the right to the right one cell.
                                ]
                                +++++[-<----------->]>[->+<]<
                            ] [ End If: c == '[' ]]

                            +++++[-<++++++>]<->>
                        ] [ End If: c != '>' ]
                        <
                        [
                            >+[->>+]
                            [ Go to the current cell pointer. ]
                            ->-[+>>-]
                            [ Move it once to the right. ]
                            >>+~

                            +[-<<+]
                            [ Go ALL the way back to the beginning of the tape. ]
                            -

                            -[+>>-]
                            [ Go to the instruction pointer. ]
                            +
                            
                            >>
                            [ Go to the zero following the instruction. ]
                        ] [ End If: c == '>' ]

                        [ Undo the subtraction at the beginning of this block and exit the loop. ]
                        <++>>
                    ] [ End If: c != '<' ]
                    <
                    [
                        >+[->>+]
                        [ Go to the current cell pointer. ]
                        ->-[+>>-]
                        [ Move it once to the left. ]
                        <<+~

                        +[-<<+]
                        [ Go ALL the way back to the beginning of the tape. ]
                        -

                        -[+>>-]
                        [ Go to the instruction pointer. ]
                        +
                            
                        >>
                        [ Go to the zero following the instruction. ]
                    ] [ End If: c == '<' ]

                    [ Undo the subtraction at the beginning of this block and exit the loop. ]
                    <++++++++++++++>>
                ] [ End If: c != '.' ]
                <
                [
                    >+[->>+]
                    [ Go to the current cell pointer. ]
                    ->-[+>>-]
                    [ Print the value there. ]
                    +>.~<

                    +[-<<+]
                    [ Go ALL the way back to the beginning of the tape. ]
                    -

                    -[+>>-]
                    [ Go to the instruction pointer. ]
                    +
                    
                    >>
                    [ Go to the zero following the instruction. ]
                ] [ End If: c == '.' ]

                [ Undo the subtraction at the beginning of this block and exit the loop. ]
                <+>>
	        ] [ End If: c != '-' ]
	        <
	        [
	            >+[->>+]
                [ Go to the current cell pointer. ]
                ->-[+>>-]
                [ Decrease the value there. ]
                +>-~<

                +[-<<+]
                [ Go ALL the way back to the beginning of the tape. ]
                -

                -[+>>-]
                [ Go to the instruction pointer. ]
                +
                
                >>
                [ Go to the zero following the instruction. ]
	        ] [ End If: c == '-' ]

            [ Undo the subtraction at the beginning of this block and exit the loop. ]
            <+>>
	    ] [ End If: c != ',' ]
	    <
	    [
	        >+[->>+]
            [ Go to the current cell pointer. ]
            ->-[+>>-]
            [ Place the next input into the current cell. ]
            +>~,~<

            +[-<<+]
            [ Go ALL the way back to the beginning of the tape. ]
            -

            -[+>>-]
            [ Go to the instruction pointer. ]
            +
            
            >>
            [ Go to the zero following the instruction. ]
	    ] [ End If: c == ',' ]

        [ Undo the subtraction at the beginning of this block and exit the loop. ]
        <+>>
	] [ End If: c != '+' ]
	<
	[
        >+[->>+]
        [ Go to the current cell pointer. ]
        ->-[+>>-]
        [ Increment the current cell. ]
        +>+~<

        +[-<<+]
        [ Go ALL the way back to the beginning of the tape. ]
        -

        -[+>>-]
        [ Go to the instruction pointer. ]
        +
        
        >>
        [ Go to the zero following the instruction. ]
	] [ End If: c == '+' ]

    [ Undo the subtraction at the beginning of this block. ]
    <+++++++

    <->>+>>[-<+>]
    [
        Move the instruction to the right back into its place
        and move the instruction pointer to the right once.
    ]
    <~
]








    




