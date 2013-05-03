#!/usr/bin/python
import numpy as np

def main():
    np.random.seed(1000)
    ndays = 30
    nteachers = 50
    npastries = 100 # lots of pastries!
    # initialize sales numbers randomly, but they will be the same from run to run since we've set the seed
    salesnumbers = np.random.randint(0, 10, (npastries, nteachers, ndays))
    profitperpastry = np.random.rand(npastries)
    # compute profits here

    
    # Generate a profit 3D array by multiple the per-unit profit with sold number
    profit = np.zeros( (npastries, nteachers, ndays) )    
    for item in range(0,npastries):
        profit[item] = profitperpastry[item] * salesnumbers[item,:,:]

    #summary the everyday profit for every teacher    
    profitTeacher = sum(profit[:,:, :])

    #total profit under each teacher's supervision
    profitsperteacher = np.zeros(nteachers)
        
    for item in range(0, nteachers):
        profitsperteacher[item] = sum(profitTeacher[item, :])
        
    #total profit    
    totalprofits = sum(profitsperteacher[:])
    ##############################################################

    print 'Total profits: {}'.format(totalprofits)
    print 'Profits per teacher: {}'.format(profitsperteacher)

if __name__ == '__main__':
    main()
