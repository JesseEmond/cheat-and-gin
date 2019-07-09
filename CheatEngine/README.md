# CheatEngine Logic
A cheat engine works in 3 main steps:

1. Finding potential addresses for the target value;

2. Refining the matches based on the updated value;

3. Modifying the matched addresses.

## Searching for Matches
Handled by `CheatEngine`'s `search`. We go through all the cheatable memory pages of the target process
(readable and writable). We then read the memory of all those pages, searching for offsets within the pages
where we can find our target value (effectively finding potential addresses for our target value).

## Refining the Search
Handled by `CheatEngine`'s `narrowDown`. We then interact with the target process such that the target value changes. We then rerun our search, though
**only looking at addresses that previously matched**, and only keep the addresses that *still* match.

## Modifying the Values
Handled by `CheatEngine`'s `modify`.
We can then modify the other process' memory at every remaining address in our matches with our new value.

# Platform-Specific Functionalities

## Linux
### Memory Pages
To find the read/write memory pages of a Linux process, we can read the `/proc/[pid]/maps` file lines and [parse them](https://stackoverflow.com/a/1401595)
to extract the start and end addresses of the pages along with their associated rights.

### Ptrace Attach
To read or write to a process' memory, we first need to [`ptrace attach`](http://man7.org/linux/man-pages/man2/ptrace.2.html) to
it (handled through RAII by `LinuxProcessLocker`).

### Read Memory
To read memory from another process, we can read from the `/proc/[pid]/mem` file directly at the right offsets (based
on the extracted page starts/sizes).

### Write Memory
To modify another process' memory, we use [`process_vm_writev`](https://linux.die.net/man/2/process_vm_writev).

## Windows
### Getting Access
To be able to interact with the other process, we open it using the [`PROCESS_ALL_ACCESS`](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684880(v=vs.85).aspx)
flag.

### Memory Pages
We can cycle through the memory pages of the process by calling the [`VirtualQueryEx`](https://msdn.microsoft.com/en-us/library/windows/desktop/aa366907(v=vs.85).aspx)
function.

### Read Memory
We can read memory by calling [`ReadProcessMemory`](https://msdn.microsoft.com/en-us/library/windows/desktop/ms680553(v=vs.85).aspx).

### Write Memory
We can write to memory by calling [`WriteProcessMemory`](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681674(v=vs.85).aspx).
