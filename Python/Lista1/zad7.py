def concreteIllness(illness, patient):
    list(illness)
    list(patient)
    for i in range(0, len(illness)):
        if(illness[i]!="*"):
            if(illness[i]!=patient[i]):
                return False
    return True

def isIll(patient):
    choroba = {"HIV": "a**a******", "AIDS": "c*****cca", "Koronawirus":"*ab******", "Cholera": "******ccc" }
    confirmedDes = []
    if(concreteIllness(choroba["HIV"],patient)):
        confirmedDes.append("HIV")
    if (concreteIllness(choroba["AIDS"], patient)):
        confirmedDes.append("AIDS")
    if (concreteIllness(choroba["Koronawirus"], patient)):
        confirmedDes.append("Koronawirus")
    if (concreteIllness(choroba["Cholera"], patient)):
        confirmedDes.append("Cholera")
    return confirmedDes


patients = ["aababacaa", 'caaaabcca', 'aaabbcbac', 'acababbaab', 'acababccc', 'aaabcbccc']
for i in range (0,len(patients)):
    x=isIll(patients[i])
    if(len(x)>0):
        print("patient ",i, " has: ",x)
    else:
        print("patient ", i, " is OK")
