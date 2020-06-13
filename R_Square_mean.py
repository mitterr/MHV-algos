from scipy import stats
import numpy as np

f = open('input.txt', 'r')
a =  sorted(f.read().split(' '))
b =  sorted(f.read().split(' '))
frequency = a = int(f.readline())


def calc_determ_const(a, b): # a: time values from solve_nk, b : time values from solve_nk_logk
    slope, intercept, r_value, p_value, std_err = stats.linregress(a,b)
    print ("r-squared:", r_value**2)
    return slope

def calc_sq_mean(a, b, c, frequency): # a: time values from solve_nk, b : time values from solve_nk_logk, frequency - difference between consecuent k, c - constant from linear regression
    arr = np.ones(a.shape[0])*frequency
    logs = np.log(np.cumsum(arr))
    res = a*logs
    return np.mean((1 - c * b / res )**2)


def calc_sq_mean(a,b, calc_determ_const(a, b), frequency)




