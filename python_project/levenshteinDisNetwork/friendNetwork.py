#!/usr/bin/python

#Author: WindInHisHair
__author__ ='WindInHisHair'
#Date: Dec 05, 2012
__date__ ='2012-12-05'

from time import time
import copy


#global variables
network = set()
dic = {}
lstSet = set()

# def leveDis(str1, str2):
# 	'''Calcualte the levenshtein distance from str1 to str2

# 	This function is unefficient. with python there is more efficient way to handle this problem'''

# 	dis = -1  #if return -1 shows the failure

# 	#assume both the str1 and str2 are words including letter more than zero

# 	row = len(str1)
# 	col = len(str2)
# 	temp = [0] * max(row, col)

# 	for item in range(min(row, col)):
# 		if str1[item] != str2[item]:
# 			temp[item] = 1
	
# 	item = item +1
# 	temp[item:] = [1 for i in range(len(temp) - item)]
	
# 	mtx = [[0] * col for r in range(row)]
	

# 	for i in range(row):
# 		for j in range(col):

# 			if str1[i] == str2[j]:
# 				temp = 0
# 			else:
# 				temp = 1

# 			if i == 0:
# 				v1 = j+2
# 				if j != 0:
# 					temp = temp + j
# 			else:
# 				v1 = mtx[i-1][j] +1
# 			if j == 0:
# 				v2 = i +2
# 				if i != 0:
# 					temp = temp + i
# 			else:
# 				v2 = mtx[i][j-1] + 1

# 			if i != 0 and j != 0:
# 				temp = temp + mtx[i-1][j-1]

			
# 			mtx[i][j] = min(v1, v2, temp)
# 			#print v1, v2, v3


# 	dis = mtx[row - 1][col - 1]
# 	#print mtx
# 	return dis

# def findFriend(str, level):
# 	# the original function for finding all friend with Dijkstra algorithm
# 	# this funciton is unefficient with python becuase of two reason:
# 	# 		1) the levenshtein distance should not be calcualted , instead, the friend should be actively generated
#     #		2) loop all the node in whole list is very unefficient
#     # not recommendated

# 	fromSet =set()
 
# 	# print 'level: ', level, 'network: ', len(network)

# 	if level in dic.keys():
# 		fromSet.update(dic[level])
# 		if not fromSet:
# 			return 0
# 	else:
# 		return 0
# 	num = 0

# 	for item in fromSet:
# 		# if leveDis(str, item) == 1:
# 		if isFriend(str, item) == 1: 
# 			network.add(item)			
# 			num = num +1
# 			findFriend(item, level+1)
# 	return num

def getFriend(seed):
	''' The optimized function for finding friend network.

	This funciotn is same as th findFriend, but optimized according to python features.
	It also work recursively'''
	# print len(lstSet)
	if not seed:
		return set()
	tmp = set([])
	s = [generateFriend(i) for i in seed if i not in network]
	s = set([i for j in s for i in j ])

	tmp.update(s.intersection(lstSet))
	network.update(seed)
	getFriend(copy.deepcopy(tmp))

	return copy.deepcopy(tmp)

def generateFriend(base):
	''' Given a word, generate friends within levenshtein distance 1.

	This is the magic part for this project. Instead of  calcuating the levenshtein distance, the function generates all possible friend and compare.
	Because python is very strong and efficient in set operation, then the performance is greatly improved.'''

	l =[]
	l.extend('abcdefghijklmnopqrstuvwxyz')
	lst = set([])
	#insert all possible letter at all possible position
	lst.update( [c.join([base[:i], base[i:]]) for c in l for i in range(len(base))])
	lst.update( [base+i for i in l] )

	
	#delete each letter in given word
	lst.update( [''.join([base[:i], base[i+1:] ]) for i in range(len(base))] )	
	#replace each letter with all possible letters
	lst.update( [c.join([base[:i], base[i+1:]]) for c in l for i in range(len(base))])
	

	return copy.deepcopy(lst)

# def isFriend(str1, str2):
# 	#this function is used to replace the leveDis fucntion, by check two words whether friends
# 	#Unfortunately, it is still unefficient
# 	if len(str1) > len(str2):
# 		target = str1
# 		base = str2

# 	else:
# 		target = str2
# 		base = str1

# 	l =[]
# 	l.extend('abcdefghijklmnopqrstuvwxyz')
# 	lst = set([])
# 	lst.update( [c.join([base[:i], base[i:]]) for c in l for i in range(len(base))])
# 	lst.update( [base+i for i in l] )

# 	# print len(lst)
# 	lst.update( [''.join([base[:i], base[i+1:] ]) for i in range(len(base))] )	
# 	# print len(lst)
# 	lst.update( [c.join([base[:i], base[i+1:]]) for c in l for i in range(len(base))])
# 	# print len(lst)

# 	if target in lst:
# 		return 1
# 	else:
# 		return 0

def main():

	inputfile = 'word_list.txt'
	root = 'causes'
	

	
	
	lstSet.update(set( open(inputfile).read().splitlines()))
	t = time()
	#---------------------------------------------------------------------------
	# generate a dicntionary with key is the distance from "causes", the value is the set of word with the same distance
	# again it is unefficient
	# for it in lstSet:
	# 	key = leveDis(root,it)
	# 	if key not in dic.keys():
	# 		dic[key] = set([])
	# 		dic[key].add(it)
	# 	else:
	# 		dic[key].add(it)
	#---------------------------------------------------------------------------
	# 
	# network.update(dic[1])	#add the direct friend by looking up in dictionary
	#original function to find friends network
	# for item in network:
	# 	findFriend(item, 2)
	#---------------------------------------------------------------------------
	#I try to optimizing the function by discarding "for loop", but not successful

	# for index in dic.keys():
	# 	if index == 0 or index == 1:
	# 		continue
	# 	else:
	# 		if index -1 not in dic.keys():
	# 			break
	# 		else:
	# 			l = [generateFriend(i) for i in dic[index]]
	# 			l = set([i for j in l for i in j])
	# 			network.update(l.intersection(lstSet))


	#Above is the functions that efficient in algorithms design but unefficient in implementation because of ignoring the python feature.


	#---------------------------------------------------------------------------
	# same Dijkstra algorithm with loop implemenation but optimized by replace 'for loop expression'	
	# select = set([])
	# temp = set([])
	
	# select.add(root)
	# l = [generateFriend(item) for item in select]
	# l = set([i for j in l for i in j])
	
	# while True:
	# 	if len(select) == 0:
	# 		break		
	# 	l = [generateFriend(item) for item in select if item not in network]
	# 	l = set([i for j in l for i in j])
	# 	temp = l.intersection(lstSet)
	# 	network.update(select)
	# 	select.clear()
	# 	select.update(temp)
	# 	temp.clear()
		# print 'network: ', len(network)
	#---------------------------------------------------------------------------
	
	#optimized implementation by recursive function, I prefer recursively implementation

	getFriend(set([root]))

	print time()-t
	print 'the network is : ', len(network)


if __name__ =="__main__":
	main()