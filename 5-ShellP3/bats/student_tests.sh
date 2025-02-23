#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

# Test cd with no argument
@test "cd with no argument does not change directory" {
    original_pwd=$(pwd)

    cd /tmp
    mkdir -p dsh-test
    run "${original_pwd}/dsh" <<EOF
cd
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/tmpdsh3>dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "cd with one argument changes to the specified directory" {
    original_pwd=$(pwd)

    cd /tmp
    mkdir -p test-dsh
    run "${original_pwd}/dsh" <<EOF
cd test-dsh
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="/tmp/test-dshdsh3>dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "cd to a non-existent directory prints an error" {
    run ./dsh <<EOF
cd non_existent_directory
EOF

     stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="cdfailed:Nosuchfileordirectorydsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "external command ls prints the directory contents" {
    run ./dsh <<EOF
ls
EOF

     stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilereadme.mddsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "non-existent command gives error" {
    run ./dsh <<EOF
not_exists
EOF
     stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="execvp:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "non command gives error" {
    run ./dsh <<EOF

EOF
     stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>warning:nocommandsprovideddsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "exit command works as intended" {
    run ./dsh <<EOF
exit
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 249 ]
}

@test "dragon command works as intended" {
    run ./dsh <<EOF
dragon
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>@%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%@%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%@%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%@%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%@%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%@dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}



@test "command with an argument works as intended" {
    run ./dsh <<EOF
uname -s
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="Linuxdsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "command with multiple arguments works" {
    run ./dsh <<EOF
uname -s -r
EOF
    stripped_output=$(echo "$output" | grep -o "Linux 5.15")
    expected_output="Linux 5.15"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "echo with spaces preserved" {
    run ./dsh <<EOF
echo hello,    world
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="hello,worlddsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "successful return code for valid command" {
    run ./dsh <<EOF
cd
rc
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>dsh3>0dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "return code of 2 for command that doesnt exist" {
    run ./dsh <<EOF
not_exsist
rc
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="execvp:Nosuchfileordirectorydsh3>dsh3>dsh3>22dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "return code of 1 for a failed command" {
    run ./dsh <<EOF
cat not_exsist
rc
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="cat:not_exsist:Nosuchfileordirectorydsh3>dsh3>1dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "return code of 13 for permission denied" {
    run ./dsh <<EOF
./
rc
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="execvp:Permissiondenieddsh3>dsh3>dsh3>22dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "return code of -1 for no commands" {
    run ./dsh <<EOF

rc
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>warning:nocommandsprovideddsh3>-1dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "Not a directory error" {
    touch not_dir_file
    run ./dsh <<EOF
cd not_dir_file
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="cdfailed:Notadirectorydsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "return code of 22 for a einval" {
    touch not_dir_file
    run ./dsh <<EOF
cd not_dir_file
rc
rm not_dir_file
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="cdfailed:Notadirectorydsh3>dsh3>22dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}



@test "pipe works for test 1" {
    run ./dsh <<EOF
ls | grep .c
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="dragon.cdsh_cli.cdshlib.cdsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe works for test 2" {
    run ./dsh <<EOF
ls | cat
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilereadme.mddsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe works for test 3" {
    run ./dsh <<EOF
ls | grep .c | wc -l
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="3dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "pipe fails for test 4" {
    run ./dsh <<EOF
ls | invalid
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="execvp:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "pipe works for test 5" {
    run ./dsh <<EOF
echo hello world | tr 'a-z 'A-Z'
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="HELLOWORLDdsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe works for test 6" {
    run ./dsh <<EOF
echo |  grep test
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe works for 8 commands" {
    run ./dsh <<EOF
ls | ls | ls | ls | ls | ls | ls | ls 
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilereadme.mddsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe fail for 9 commands" {
    run ./dsh <<EOF
ls | ls | ls | ls | ls | ls | ls | ls | ls
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')    expected_output="dsh3>error:pipinglimitedto8commandsdsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "pipe first command runs when no second" {
    run ./dsh <<EOF
ls | 
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')    
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilereadme.mddsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipe second command runs when no first" {
    run ./dsh <<EOF
 | ls 
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="batsdragon.cdshdsh_cli.cdshlib.cdshlib.hmakefilereadme.mddsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
