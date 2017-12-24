# rick-pic
kernel module that hijacks opening any .png file and shows
a rick gif instead.

## usage
You probably shouldn't use this at all(!), but if you want to do:

```sh
$ make
$ sudo insmod rick_pic.ko
```

To remove the module do:
```sh
$ sudo rmmod rick_pic
```

You can read output from the module by reading the
`/var/log/messages` file or using `dmesg` which seems to do
the same thing(?):

```sh
$ cat /var/log/messages | tail

... prints out something from the rick_pic module ...

-- or --
$ dmesg | tail
... also  prints out something from the rick_pic module ...
```

Running this depends on having the linux header files on your machine:

```sh
$ sudo apt install linux-headers-(uname -r)-generic
```
## Acknowledgments
super huge shoutout to the very awesome
[jvns](https://jvns.ca/blog/2014/09/18/you-can-be-a-kernel-hacker)
 -- this whole thing is just totally stolen from her.

 Also learned a lot from going through the first few chapters of
 [The Linux Kernel Module Prgramming
 Guide](https://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN245)
 which is fun to read, and despite being pretty old the examples compiled 
 great!

## License
GPL
