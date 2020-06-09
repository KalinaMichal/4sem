from pip._vendor.distlib.compat import raw_input

arab=0
rome = str(raw_input())
list(rome)
last=2000
last2 = last
last3 = last2
for i in range(len(rome)):
    if(rome[i]=="I"):
        if(last==1 and last2==1 and last3==1):
            arab = "zle dane"
            break;
        if (last != 1 and last2 == 1):
            arab = "zle dane"
            break;
        last3 = last2
        last2=last
        last=1
        arab+=1
    if(rome[i]=="V"):
        if(last2==1):
            arab = "zle dane"
            break;
        if(last==1):
            arab+=3
        else:
            arab+=5
        last3 = last2
        last2=last
        last=5
    if(rome[i]=="X"):
        if(last==10 and last2==10 and last3==10):
            arab = "zle dane"
            break;
        if((last==1 and last2==1) or last==5):
            arab = "zle dane"
            break;
        if(last==1):
            arab+=8
        else:
            arab+=10
            last3 = last2
        last2=last
        last=10
    if(rome[i]=="L"):
        if(last<10 or (last<=10 and last2<=10)):
            arab = "zle dane"
            break;
        last3 = last2
        last2 = last
        last=50
        arab+=50
    if(rome[i]=="C"):
        if(last2!=2000 or (last!=10 and last<100)):
            arab = "zle dane"
            break;
        last3 = last2
        last2 = last
        last=100
        arab+=100
    if(rome[i]=="D"):
        if(last2!=2000 or (last!=100 and last<500)):
            arab = "zle dane"
            break;
        last3 = last2
        last2 = last
        last=500
        arab+=500
    if(rome[i]=="M"):
        if(last2!=2000 or (last!=500 and last<1000)):
            arab = "zle dane"
            break;
        arab+=1000
        last3 = last2
        last2=last
        last = 1000
    if(rome[i]!="M" and  rome[i]!="I" and  rome[i]!="V" and  rome[i]!="X"and  rome[i]!="L"and  rome[i]!="C" and rome[i]!="D"):
        arab="zle dane"
        break;
print(arab)
