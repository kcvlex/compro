print(['<','>','='][(lambda lis:(lis[0]==lis[1])*2+(lis[0]>lis[1]))(input().split())])
