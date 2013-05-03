#!/usr/bin/python
import sys


#author:WindInHisHair 
__author__ = "WindInHisHair"
#Date: Nov 30, 2012
__date__ = "2012-11-30"
#descriptions: generate a path according to attractions with least cost

#the three dictionaries to efficiently searching
flightDic = {}
cityAttrDic = {}
attrCityDic = {}




def loadInfo(flightCost_file, flightDes_file, startCity, attractions):
    ''' This function is used to deal with user input:

    1 check the name of city and attractions
    2 loading information from file into the dictionary. '''

    #reading information from files
    try:
        for line in open(flightCost_file,"r"):
            start, des, cost1, cost2  = line.strip().split(",")
            flightDic[start+des] = cost1
            flightDic[des+start] = cost2
    except:
        sys.exit("error when open file" + flightCost_file)
    
    
    try:   
        for line in open(flightDes_file,"r"):
            line = line.strip().split(',')
            city = line[0]
            attraction = line[1:]                    
            cityAttrDic[city] = attraction
    except:
        sys.exit("error when open file " + flightDes_file)
    
    #checking the input city name and attractions
    for c in cityAttrDic.keys():
        for item in cityAttrDic[c]:
            if item not in attrCityDic.keys():
                attrCityDic[item] = set([c])
            else:
                attrCityDic[item].add(c)
    #check the there is a flight for the input city
    
    if startCity not in [item[0:3] for item in flightDic.keys()]:
        print '************************ FLIGHT ERROR **********************************'
        sys.exit("PLEASE check input CITY is INVOLVED in vacation_flights.csv")
    #make sure attractions existence
    for item in attractions:
        if item not in attrCityDic.keys():              
            print '*********************ATTRACTION ERROR******************************'
            sys.exit("PLEASE check input ATTRACTIONS SAME AS IN vacation_destinations.csv file")
    
def DijkPath(start, des, prev):
    ''' Find the shortest path with Dijkstra Algorithm from proint "start" to point "des"
    
    This function is the fundamental tool in this project, After planing, the functions is used to
    calculate the cost from the given starting point to the destination point.'''
    
    
    path = []
    pathList = []
    
    prev.add(start)
    
    #find the least cost from starting point with one step   
    num = 0
    for item in flightDic.keys():
        if item[:3] == start:
            if num == 0:
                num = flightDic[item]
            elif num > flightDic[item]:
                num = flightDic[item]
    
    #apply Dijkstra algorithm 
    if start+des not in flightDic.keys():
        #destination does not lie in current connection, then finding it from neighbors     
        for item in flightDic.keys():
            if item[:3] == start and item[3:] not in prev:
                localSet = set([])
                localSet.update(prev)
                p = DijkPath(item[3:], des, localSet)
                if len(p) != 0:
                    path.append(int(p[0]) + int(flightDic[item]))                    
                    path.append(item[3:])
                    path.extend(p[1:])
                    pathList.append(path)
                    path = []
        if not pathList:
            return []
        else:
            num = pathList[0][0]        
            for i in range(len(pathList)):
                if num > int(pathList[i][0]):                             
                    num = pathList[i][0]
            
            path.append(num)
            for i in range(len(pathList)):
                if pathList[i][0] == num:
                    path.extend(pathList[i][1:])
            return path                 
    else:
        # destination lies in current connection,
        #if cost least,then choose, if not try to find less cost path from neighbor
        path.append(int(flightDic[start+des]))
        path.append(des)
        if flightDic[start+des] == num or not pathList:            
            return path
        else:            
            num = pathList[0][0]        
            for i in range(len(pathList)):
                if num > int(pathList[i][0]):                             
                    num = pathList[i][0]
            if flightDic[start+des] < num:
                return path
            else:
                if path not in pathList:
                    pathList.append(path)
                p =[]
                for i in range(len(pathList)):
                    if pathList[i][0] == num:
                        p.extend(pathList[i][1:])
                return p
                    
    
        
    
        
def generatePath(currentCity, startCity, attraction):
    '''The funtion used to generate a path with least cost and including  all attractions
    
    basic idea of the funtion is:
    1) find a path from staring city the including all attraction;
    2) once we meet all the attractions, we plan to go back.
    then the solution is the path with least cost.'''
        
    #when all attractions is visited, and there is no back way to starting city,
    #then discard current path by return an empty list
    pathList = []
    path = []

    #test whether all the attraction is included, if so, plan to go back 
    if not attraction:
        return DijkPath(currentCity, startCity, set([currentCity]))
    
    #city with attractions that we demand 
    citySet = set()
    for item in attraction:        
        for each in attrCityDic[item]:
            if each == startCity:
                continue
            citySet.add(each)
    
    #try to go to every city with at least one attraction, the least cost path will be found    
    for item in citySet:
        localAttr = []
        localAttr.extend(attraction)
        for eachAttr in cityAttrDic[item]:
            if eachAttr in localAttr:
                localAttr.remove(eachAttr)
        
        #generate a least-cost path         
        p1 = DijkPath(currentCity, item, set([currentCity]))
        
        #keep going to more city, if all the attractions is included then stop to plan for going back
        p2 = generatePath(item, startCity, localAttr)
        
        path.append(int(p1[0]) + int(p2[0]))            
        path.extend(p1[1:])           
        path.extend(p2[1:])      
      
        pathList.append(path)
        path = []
    
    if not pathList:
        return []
    else:
        num = pathList[0][0]        
        for i in range(len(pathList)):
            if num > int(pathList[i][0]):                             
                num = pathList[i][0]
        
        path.append(num)
        for i in range(len(pathList)):
            if pathList[i][0] == num:
                path.append(pathList[i][1:])
        return path                     
    

            
    
def proLst(lst):
    l = []
    for i in range(1,len(lst)):
        if len(lst[i]) > 3 :
            tmp = proLst(lst[i])
            l.extend(tmp)
        else:
            l.append(lst[i])
    return l
def showPro():
    print "waiting for the result! ->"
    for i in range(1000000):
        print '-> '
        time.sleep(0.5)
        
def main():   
    
    if len(sys.argv) < 3:
        sys.exit("incorrect parameter number.")
    city = sys.argv[1]
    attractions = sys.argv[2:]
      
    flightCost_file = 'vacation_flights.csv'
    flightDes_file = 'vacation_destinations.csv'
    
    #load information from files
    loadInfo(flightCost_file, flightDes_file,city, attractions)
    #print flightDic, cityAttrDic, attrCityDic
   
    print 'Please wait ...'
    
    path = generatePath(city, city, set(attractions))
    print 'Attractions List:'
    for item in attractions:
        print item, ' : ', attrCityDic[item]
    print ''    
    
    print '*****************************************'
    print 'TIPS:  [] means alternative path: '
    print 'For example: [a b c a]  and [a b d a] both work, then [a, [b, c,a],[b d a]] '
    print 'It generate same path sometimes,'
    
    print '*****************************************'

    #To reduce the redunancy will introduce another project 
    print 'TOTAL COST: ', path[0]
    print 'the city: ', city, ' the least cost path :'
    print ''
    for i in range(1,len(path)):
        print'---------------------------------------'
        print path[i]
    print'---------------------------------------'
    
     
if __name__ == '__main__':
    main()
    
