games = ['''[Event "Live Chess"]
[Site "Chess.com"]
[Date "2022.11.01"]
[Round "?"]
[White "elovena"]
[Black "leeviloikkanen"]
[Result "0-1"]
[ECO "A00"]
[WhiteElo "819"]
[BlackElo "1367"]
[TimeControl "180"]
[EndTime "12:16:24 PDT"]
[Termination "leeviloikkanen won by resignation"]

1. d3 Nf6 2. c4 g6 3. Nc3 d6 4. Bg5 Bg7 5. Nf3 c5 6. e4 Nc6 7. Be2 O-O 8. O-O h6
9. Bxf6 Bxf6 10. Nd5 Bg7 11. b3 Bxa1 12. Qxa1 e6 13. Nc3 Ne7 14. d4 cxd4 15.
Nxd4 d5 16. f3 b6 17. exd5 exd5 18. f4 dxc4 19. Ndb5 cxb3 20. axb3 Bf5 21. Rd1
Qc8 22. Nd6 Qc5+ 23. Kf1 Rad8 24. Nxf5 Nxf5 25. Re1 Rfe8 26. Na4 Qc2 27. Qb2
Qxb2 28. Nxb2 Rd2 29. Nc4 Rd4 30. Ne5 Rxf4+ 31. Kg1 Rxe5 0-1''']

#g i the game we're converting
for g in range(1):
    #n1 is where we find 1.
    n1 = 0
    #s1 tells if the previous symbol was ], s2 if it was 1
    s1 = False
    s2 = False
    for i in range(len(games[g])):
        if games[g][i] == "]":
            s1 = True
        elif s1 == True and games[g][i] == "1":
            s2 = True
            s1 = False
        elif s2 == True and games[g][i] == ".":
            print(games[g][i-1])
            print(games[g][i])
            print(games[g][i+2])
            n1 = i
            break
        elif games[g][i] == " " or games[g][i] == "\n":
            pass
        else:
            s1 = False
            s2 = False
    for i in range(n1, len(games[g])-n1):
        print(games[g][n1+i])