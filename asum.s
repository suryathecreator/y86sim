init: irmovl Stack, %esp
      irmovl Stack, %ebp
      call Main               # Execute main program
      halt                    # Terminate program
array: .long 0xd
       .long 0xc0
       .long 0xb00
       .long 0xa000
Main:  pushl %ebp
       rrmovl %esp, %ebp
       irmovl $4, %eax
       pushl %eax             # Push 4
       irmovl array, %edx
       pushl %edx             # Push array
       call Sum               # Sum(array, 4)
       rrmovl %ebp, %esp
       popl %ebp
       ret
Sum:   pushl %ebp
       rrmovl %esp, %ebp
       mrmovl 8(%ebp), %ecx   # ecx = Start
       mrmovl 12(%ebp), %edx  # edx = Count
       xorl %eax, %eax        # sum = 0
       andl %edx, %edx        # Set condition codes
       je End
Loop:  mrmovl (%ecx), %esi    # get *Start
       addl %esi, %eax        # add to sum
       irmovl $4, %ebx
       addl %ebx, %ecx        # Start++
       irmovl $-1, %ebx
       addl %ebx, %edx        # Count--
       jne Loop               # Stop when 0
End:   rrmovl %ebp, %esp
       popl %ebp
       ret