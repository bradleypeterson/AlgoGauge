package main

import (
    "fmt"
    "os/exec"
)

func main() {
    // Specify the path to the compiled Rust binary
    cmd := exec.Command("./Rust/my_rust_program")


    // Start the command (without waiting for it to finish)
    if err := cmd.Start(); err != nil {
        fmt.Printf("Error starting Rust child process: %v\n", err)
        return
    }

    // Get the PID of the child process
    fmt.Printf("PID of Rust child process: %d\n", cmd.Process.Pid)

    // Wait for the process to finish and capture its output
    if err := cmd.Wait(); err != nil {
        fmt.Printf("Rust child process exited with error: %v\n", err)
    } else {
        fmt.Println("Rust child process finished successfully")
    }
}
