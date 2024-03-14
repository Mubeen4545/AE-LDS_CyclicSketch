import time
from sklearn.metrics import classification_report, confusion_matrix, accuracy_score
import pandas as pd
from sklearn.metrics import recall_score, precision_score, f1_score
import matplotlib.pyplot as plt
from sklearn import ensemble
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler  
import csv

trainFile = "Train_Set.csv"
testFile = "MixedTraffic.csv"
metricfile = ''

def writeprelabel(filename, prelabel):
    df = pd.read_csv(filename)
    df['Prelabel_T'] = prelabel
    df.to_csv(filename, index=None)

def write_metrics(metricfile, metrics_list):
    with open(metricfile, 'a', newline='') as student_file:
        writer = csv.writer(student_file)
        writer.writerow(metrics_list)

def detect(trainfile, testfile, alg):
    data = pd.read_csv(trainfile)
    train_y = data[['Label_T']]
    train_x = data[['F_Pck', 'B_Pck', 'F_P_L1', 'B_P_L1', 'F_TCP_S_C', 'B_TCP_S_C', 'A_M', 'I_M']]
    
    data_test = pd.read_csv(testfile)
    test_y = data_test[['Label_T']]
    test_x = data_test[['F_Pck', 'B_Pck', 'F_P_L1', 'B_P_L1', 'F_TCP_S_C', 'B_TCP_S_C', 'A_M', 'I_M']]

    scaler = StandardScaler()  
    train_x_scaled = scaler.fit_transform(train_x)
    test_x_scaled = scaler.transform(test_x)

    if alg == 'DT':
        clf = DecisionTreeClassifier(criterion='entropy', max_depth=10, min_samples_leaf=1, random_state=None, splitter='best')
        print('alg: DT')
    elif alg == 'RF':
        clf = ensemble.RandomForestClassifier(n_estimators=10)
        print('alg: RF')
    elif alg == 'SVM':
        clf = SVC(kernel='linear')
        print('alg: SVM')

    start_time = time.time()
    clf = clf.fit(train_x_scaled, train_y.values.ravel())
    end_time = time.time()
    training_time = end_time - start_time
    print("Training time for {} classifier: {:.2f} seconds".format(alg, training_time))

    start_time = time.time()
    y_predict = clf.predict(test_x_scaled)
    end_time = time.time()
    classification_time = end_time - start_time
    print("Classification time for {} classifier: {:.2f} seconds".format(alg, classification_time))
    
    writeprelabel(testfile, y_predict)

    metrics_list = []
    metricfile = testfile + '_Metrics_TC.csv'
    metrics_list.append(testfile)
    precision = precision_score(test_y, y_predict, average='weighted')
    recall = recall_score(test_y, y_predict, average='weighted')
    F1 = f1_score(test_y, y_predict, average='weighted')
    accuracy = accuracy_score(test_y, y_predict)
    
    tn, fp, fn, tp = confusion_matrix(test_y, y_predict).ravel()
    FPR = fp / (fp + tn)
    FNR = fn / (fn + tp)

    metrics_list.append(str(precision))
    metrics_list.append(str(recall))
    metrics_list.append(str(F1))
    metrics_list.append(str(FPR))
    metrics_list.append(str(FNR))
    metrics_list.append(str(accuracy))

    print('recall:' + str(recall))
    print('precision: ' + str(precision))
    print('F1 score: ' + str(F1))
    print('False Positive Rate (FPR): ' + str(FPR))
    print('False Negative Rate (FNR): ' + str(FNR))
    print('Accuracy: ' + str(accuracy))
    
    write_metrics(metricfile, metrics_list)

    plt.figure(figsize=(15, 8))
    confusion = confusion_matrix(test_y, y_predict)
    plt.imshow(confusion, cmap=plt.cm.YlOrBr)
    classes = ['Benign Traffic', 'Attack Traffic']
    plt.xticks(range(len(classes)), classes, fontsize=16)
    plt.yticks(range(len(classes)), classes, fontsize=16)
    plt.colorbar()
    plt.title('Model: ' + alg + '     F1-Score: ' + str(F1))
    plt.xlabel('Predicted', fontsize=16)
    plt.ylabel('Actual', fontsize=16)
    for i in range(len(confusion)):
        for j in range(len(confusion[i])):
            plt.text(j, i, confusion[i][j], ha="center", va="center", color="black")
    plt.savefig(alg + ' AB_ITC_Confusion.pdf')

def dtct():
    detect(trainFile, testFile, 'DT')
    detect(trainFile, testFile, 'RF')
    detect(trainFile, testFile, 'SVM')

if __name__ == '__main__':
    dtct()
