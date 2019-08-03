Write the C preamble
+++++++++++++++++++++++++++++++++++.++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+++++.-----------.+++++++++.+++++++++.-----------------.+.-------------------------------------------------------------------.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+.----------------.+++++.++++++.-----------------------------------------------------------------.++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.----------------------------------------------------------------------.------------------------.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+++++.-------.+++++++++++++++++.----------------------------------------------------------------------------------.++++++++++++++++++++++++++++++++++++++++++++++++++.+++++++++.------------------------------------------.-....+++++++++++++++++++++++++++++++++++++++++++++.----------------------------------.-------------------------------------------------.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+++++.++++++.------------------------------------------------------------------------------------.++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.++++.--.-----------------------------------------------------.-------------.+++++++++++.-------------------------------------------------.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+++++.++++++.------------------------------------------------------------------------------------.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.------------.++++++++.+++++.----------------------------------------------------------------------.+.++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.-----------------------------------------------------------------------------------------------------------------.

Clear current memory
[-]

We want the tape to look like this:
 \t 0 1 0 0 1 0
        ^
The last 1 is the "searching for symbol" flag
If we detect it we set it to 0 and output C code for a symbol
That way we can build a list of things a character isn't
and when we finally find something we know it isn't not
we can assume we have the correct symbol
+++++++++>>+>>>+~<<

Until the end of the file
Assuming 255 = EOF
,----------[++++++++++


>++++++[-<------->]<-
[
    Not a plus sign
    -[
        Not a comma

        -[
            Not a minus sign

            -[
                Not a period

                >+++++++[-<-->]<
                [
                    Not a left angle bracket

                    --[
                        Not a right angle bracket

                        >+++++++++[-<--->]<--
                        [
                            Not a open bracket

                            --[
                                Not a closing bracket

                                [-] At this point it's not a useful symbol
                                >>-<<
                            ]
                            >>[<<
                                A closing bracket

                                Decrease the tab count
                                <-

                                Print the correct number of tabs
                                [-<+<.>>]

                                Reset after printing
                                <[->+<]>>>

                                Print the close bracket code
                                +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.-------------------------------------------------------------------------------------------------------------------.

                                Remove the "searching for symbol" flag
                                [-]>-
                            ]<<
                        ]
                        >>[<<
                            An open bracket

                            Print the correct number of tabs
                            <[-<+<.>>]

                            Reset after printing
                            <[->+<]>

                            Increase the tab count
                            +>>

                            Print the open bracket code
                            +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.---------------.+.+++.-------.-------------------------------------------------------------.++++++++++++++++++++++++++++++++++++++++++.+++++++++.+++++++++++++++++++++.++++.--.---------------------.----------------------------------------------------.++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.-----------------------------------------------------------------------------------------------------------------.

                            Remove the "searching for symbol" flag
                            [-]>-
                        ]<<
                    ]
                    >>[<<
                        A right angle bracket

                        Print the correct number of tabs
                        <[-<+<.>>]

                        Reset after printing
                        <[->+<]>>>

                        Print the right angle bracket code
                        ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.++++.--.-----------------------------------------------------------------------..++++++++++++++++.-------------------------------------------------.

                        Remove the "searching for symbol" flag
                        [-]>-
                    ]<<
                ]
                >>[<<
                    A left angle bracket

                    Print the correct number of tabs
                    <[-<+<.>>]

                    Reset after printing
                    <[->+<]>>>

                    Print the left angle bracket code
                    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.++++.--.---------------------------------------------------------------------..++++++++++++++.-------------------------------------------------.

                    Remove the "searching for symbol" flag
                    [-]>-
                ]<<
            ]
            >>[<<
                A period

                Print the correct number of tabs
                <[-<+<.>>]

                Reset after printing
                <[->+<]>>>

                Print the period code
                ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.++.---------.+++++.++++++.--------------.--------------------------------------------------------------.------.+++.++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.-----------------------------------------------------------------.++++++++++.++++++++++++++++++++++++++++++++++++++.+++++++++.+++++++++++++++++++++.++++.--.---------------------.----------------------------------------------------.++++++++++++++++++.-------------------------------------------------.

                Remove the "searching for symbol" flag
                [-]>-
            ]<<
        ]
        >>[<<
            A minus sign

            Print the correct number of tabs
            <[-<+<.>>]

            Reset after printing
            <[->+<]>>>

            Print the minus sign code
            ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+++++++++.+++++++++++++++++++++.++++.--.---------------------.------------------------------------------------..++++++++++++++.-------------------------------------------------.

            Remove the "searching for symbol" flag
            [-]>-
        ]<<
    ]
    >>[<<
        A comma

        Print the correct number of tabs
        <[-<+<.>>]

        Reset after printing
        <[->+<]>>>

        Print the comma code
        ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+++++++++.+++++++++++++++++++++.++++.--.---------------------.--------------------------------.++++++++++++++++++++++++++++++++++++++++++.--.+++++++++++++++.-----------------.+++++.-------.+++++++++++++++++.--------------------------------------------------------------------------.+.++++++++++++++++++.-------------------------------------------------.

        Remove the "searching for symbol" flag
        [-]>-
    ]<<
]
>>[<<
    A plus sign

    Print the correct number of tabs
    <[-<+<.>>]

    Reset after printing
    <[->+<]>>>

    Print the plus sign code
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.+++++++++.+++++++++++++++++++++.++++.--.---------------------.--------------------------------------------------..++++++++++++++++.-------------------------------------------------.

    Remove the "searching for symbol" flag
    [-]>-
]

Reset the "searching for symbol" flag
+~<<

Continue reading file
,----------]

Write the C end code
+++++++++.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.-------------.+++++++++++++++.+.---.----.------------------------------------------------------------------------------.++++++++++++++++.+++++++++++.-------------------------------------------------.+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.-------------------------------------------------------------------------------------------------------------------.