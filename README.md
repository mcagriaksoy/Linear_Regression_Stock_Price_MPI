# Linear Regression Solution for Stock Price Prediction with PThread

# Introduction
In statistics, linear regression is a linear approach to modeling the relationship between a
scalar response (or dependent variable) and one or more explanatory variables (or indepen-
dent variables). The case of one explanatory variable is called simple linear regression. In this
scenario, we are using linear regression for create a function with x,y axes. This method is
used for predict numbers and result of stock price goes higher or lower values. In this solution,
turkish airlines’ stock prices is used. The dataset is taken from kaggle. More information
can be found at: github.com/mcagriaksoy 
https://www.kaggle.com/semihyilmaz/turkish-airlines-daily-stock-prices-since-2013

# Development Environment
GCC and MPI compiler is used on Ubuntu 18.04 x64 OS.

# Comparison and Results
Due to results of experiment. PThread is much more faster than Sequantial Solution. The result numbers can be seen on the tables. Due to tables, when workload is not so much, pthread has slightly advantage than sequential one. But in more workload, the performance difference goes to 10times bigger than sequential one. It means it completes the work 10 times faster. The main difference is loop. In the implementation I have divided for loop between threads. So, If the loop is going to bigger size, the performance is increase when we compare with sequential one. But this should not be forgotten, the performance can be changed due to user interaction and algorithm’s implementation.
