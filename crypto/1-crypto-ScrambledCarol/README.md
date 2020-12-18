# Scrambled Carol

This challenge was a simple subsitution cipher effectively, despite the source doing some weird splitting of the resultant hex. 

# Solution

I first generated a frequency table for every hex pair (e.g 0e) with the code given in [main.go](main.go).

```
01 : 16
dc : 19
1d : 2
06 : 5
cd : 1
31 : 4
d7 : 25
07 : 77
c4 : 3
d0 : 12
35 : 4
04 : 11
36 : 4
d4 : 67
37 : 2
0b : 77
05 : 58
09 : 67
8f : 35
32 : 1
18 : 210
0e : 51
33 : 1
c8 : 1
03 : 29
39 : 2
00 : 15
19 : 7
0c : 113
d1 : 78
d8 : 14
3e : 16
0f : 2
30 : 3
0a : 18
3b : 9
d3 : 53
34 : 3
dd : 24
0d : 37
15 : 14
12 : 13
46 : 4
de : 1
02 : 69
c3 : 7
```

As you can see from this `18 : 210`, the hex code `18` had the highest number of occurences. So I initially thought it was the letter `e` due to the frequency of english letters.
However, this was not the case. Instead, the most occurent character was, of course, the space. After working this out, I used this one character to look for carols that start with only one letter. As we can see from the starting sequence `3b 18 0e`, that there is a letter then a space. 

After browsing a number of carols, using the spaces as an indicator, I am across 'O Holy Night', which fit the pattern. As there are multiple versions of this carol, the first one I found let me decode some of the message by hand, but not all of it. Until I found the wikipedia version (https://en.wikipedia.org/wiki/O_Holy_Night), which gave me all the characters. 

After replacing all the characters I got : `xmaswasneverasgoodasitisthpisyear`
