from sklearn.metrics import classification_report
import pandas as pd
# Import for DecisionTreeClassifier added
from sklearn.tree import DecisionTreeClassifier
import matplotlib.pyplot as plt
from sklearn.metrics import recall_score, precision_score, f1_score, confusion_matrix, accuracy_score, pair_confusion_matrix
import numpy as np
import matplotlib as mpl
from sklearn import ensemble
from sklearn import metrics
# Import for SVC (Support Vector Machine)
from sklearn.svm import SVC
import csv

trainFile="Test_Data_File.csv"
testFile= "Train_Data_File.csv"
out256="Ouput.csv"
metricfile=''

def write_metrics(metricfile,metrics_list):
    with open(metricfile, 'a', newline='') as student_file:
        writer = csv.writer(student_file)
        writer.writerow(metrics_list)

def detect(trainfile,testfile,filename,alg):
    data = pd.read_csv(trainfile,index_col=False)
    data_train =data[data['Label_AB'] == 0]
    train_y = data_train[['Label_Multi_A']]
    train_x = data_train[['F_P_L_A', 'B_P_L_A']]
    data_test=pd.read_csv(testfile,index_col=False)
    df_test=data_test[data_test['Prelabel_AB'] == 1]
    test_y = df_test[['Label_Multi_A']]
    test_x = df_test[['F_P_L_A', 'B_P_L_A']]

    if alg=='DT':
        clf = DecisionTreeClassifier(random_state=42)
    elif alg=='RF':
        clf= ensemble.RandomForestClassifier(n_estimators=10)
    elif alg=='SVM':  
        clf= SVC(kernel='linear', C=1.0, random_state=42)

    clf = clf.fit(train_x, train_y.values.ravel())  # Adjust for SVC compatibility
    y_predict = clf.predict(test_x)
    df_test['Prelabel_Multi_A'] = y_predict
    df_test.to_csv(filename, index=None)

    metricfile = testfile + '_Metrics_ATC.csv'
    metrics_list=[]
    metrics_list.append(testfile)
    Precision = precision_score(test_y, y_predict, average='weighted')
    Recall = recall_score(test_y, y_predict,average='weighted')
    F1 = f1_score(test_y, y_predict,average='weighted')

    metrics_list.append(str(Precision))
    metrics_list.append(str(Recall))
    metrics_list.append(str(F1))
    print(metrics_list)
    print('Recall:' + str(Recall))
    print('Precision: ' + str(Precision))
    print('F1 Score: ' + str(F1))
    write_metrics(metricfile, metrics_list)

    plt.figure(figsize=(15, 8))
    confusion = confusion_matrix(test_y, y_predict)
    plt.imshow(confusion, cmap=plt.cm.YlOrBr)
    indices = range(len(confusion))
    classes=['Benign','Slowloris','RUDY','S_READ','S_COMM','S_NEXT','S_DROP','A_RANGE', 'S_REQ']
    plt.colorbar()
    plt.title('Model: '+alg+'     F1-Score: '+str(F1))
    plt.xticks(indices, classes)
    plt.yticks(indices, classes)

    print(metrics.classification_report(test_y, y_predict))
    for first_index in range(len(confusion)):
        for second_index in range(len(confusion[first_index])):
            plt.text(first_index, second_index, confusion[second_index][first_index])
    plt.xlabel('Predicted')
    plt.ylabel('Actual')
    plt.savefig(alg+' ATC Confusion.pdf')

def dtct():
    detect(trainFile, testFile, out256, 'RF')
    detect(trainFile, testFile, out256, 'SVM')  
    detect(trainFile, testFile, out256, 'DT')  

if __name__=='__main__':
    dtct()
