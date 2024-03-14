from sklearn.metrics import classification_report
import pandas as pd
from sklearn.metrics import recall_score, precision_score, f1_score
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
from sklearn import ensemble
from sklearn.tree import DecisionTreeClassifier  # This line is now active
from sklearn.svm import SVC  # Added for SVM model
import csv

# specify training set file and testing set file
trainFile = "./DataSet/Test_Data_File.csv"
testFile = "./DataSet/Train_Data_File.csv"
metricfile = ''

# -------------write the predicted labels to the file-------------
def writeprelabel(filename, prelabel):
    df = pd.read_csv(filename)
    # del df['Label']
    df['Prelabel_T'] = prelabel
    df.to_csv(filename, index=None)

# -------------write the evaluation indicators to the file-------------
def write_metrics(metricfile, metrics_list):
    with open(metricfile, 'a', newline='') as student_file:
        writer = csv.writer(student_file)
        writer.writerow(metrics_list)

def detect(trainfile, testfile, alg):
    data = pd.read_csv(trainfile)
    train_y = data[['SYN_Flag_Cnt']]
    train_x = data[['Flow_Duration', 'Fwd_Pkt_Len_Mean', 'Bwd_Pkt_Len_Mean', 'Flow_Pkts/s', 'Flow_IAT_Mean', 'Flow_IAT_Std', 'Fwd_Header_Len', 'Pkt_Size_Avg', 'Active_Mean', 'Idle_Mean']]

    data_test = pd.read_csv(testfile)
    test_y = data_test[['SYN_Flag_Cnt']]
    test_x = data_test[['Flow_Duration', 'Fwd_Pkt_Len_Mean', 'Bwd_Pkt_Len_Mean', 'Flow_Pkts/s', 'Flow_IAT_Mean', 'Flow_IAT_Std', 'Fwd_Header_Len', 'Pkt_Size_Avg', 'Active_Mean', 'Idle_Mean']]

    if alg == 'DT':
        clf = DecisionTreeClassifier(criterion='entropy', max_depth=4, min_samples_leaf=15, splitter='random', random_state=None)
        print('alg: DT')
    elif alg == 'RF':
        clf = ensemble.RandomForestClassifier(n_estimators=8)
        print('alg: RF')
    elif alg == 'SVM':  
        clf = SVC(kernel='linear') 
        print('alg: SVM')

    clf = clf.fit(train_x, train_y.values.ravel())  
    y_predict = clf.predict(test_x)
    writeprelabel(testfile, y_predict)

    metricfile = testfile + '_Metrics_ITC.csv'
    metrics_list = []
    metrics_list.append(testfile)
    precision = precision_score(test_y, y_predict)
    recall = recall_score(test_y, y_predict)
    F1 = f1_score(test_y, y_predict)

    metrics_list.append(str(precision))
    metrics_list.append(str(recall))
    metrics_list.append(str(F1))

    print('recall:' + str(recall))
    print('precision: ' + str(precision))
    print('F1 score: ' + str(F1))
    write_metrics(metricfile, metrics_list)

    plt.figure(figsize=(15, 8))
    t = classification_report(test_y, y_predict, target_names=['0', '1'])
    classes = ['Benign Traffic', 'Attack Traffic']
    confusion = confusion_matrix(test_y, y_predict)
    plt.imshow(confusion, cmap=plt.cm.YlOrBr)
    indices = range(len(confusion))

    plt.xticks(indices, classes, fontsize=16)
    plt.yticks(indices, classes, fontsize=16)
    plt.title('Model: ' + alg + '     F1-Score: ' + str(F1))
    plt.colorbar()
    plt.rcParams.update({'font.size': 16})
    plt.xlabel('Predicted', fontsize=16)
    plt.ylabel('Actual', fontsize=16)


    for first_index in range(len(confusion)):
        for second_index in range(len(confusion[first_index])):
            plt.text(first_index, second_index, confusion[second_index][first_index])

    
    plt.savefig(alg + ' ITC Confusion.pdf')

# -------------select the algorithm：DT, RF, or SVM-------------
def dtct():
    detect(trainFile, testFile, 'DT')  
    detect(trainFile, testFile, 'RF')
    detect(trainFile, testFile, 'SVM')  

if __name__ == '__main__':
    dtct()
