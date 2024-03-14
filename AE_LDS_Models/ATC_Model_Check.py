from sklearn.metrics import classification_report, precision_score, recall_score, f1_score, confusion_matrix, accuracy_score
import pandas as pd
from sklearn.tree import DecisionTreeClassifier
import matplotlib.pyplot as plt
import numpy as np
import csv
from sklearn import ensemble
from sklearn.svm import SVC
import time

trainFile = "Train_Set.csv"
testFile = "MixedTraffic.csv"
out256 = "ATC_Output.csv"

def write_metrics(metricfile, metrics_list):
    with open(metricfile, 'a', newline='') as student_file:
        writer = csv.writer(student_file)
        writer.writerow(metrics_list)

def detect(trainfile, testfile, filename, alg):
    data = pd.read_csv(trainfile, index_col=False)
    data_train = data[data['Label_AB'] == 1]
    train_y = data_train[['Label_Multi_A']]
    train_x = data_train[['F_P_L_A', 'B_P_L_A']]
    
    data_test = pd.read_csv(testfile, index_col=False)
    df_test = data_test[data_test['Prelabel_AB'] == 1]
    test_y = df_test[['Label_Multi_A']]
    test_x = df_test[['F_P_L_A', 'B_P_L_A']]
    
    if alg == 'DT':
        clf = DecisionTreeClassifier(random_state=42)
    elif alg == 'RF':
        clf = ensemble.RandomForestClassifier(n_estimators=10)
    elif alg == 'SVM':
        clf = SVC(kernel='linear', C=1.0, random_state=42)
    
    start_time = time.time()
    clf = clf.fit(train_x, train_y.values.ravel())
    end_time = time.time()
    
    detection_time = end_time - start_time
    
    y_predict = clf.predict(test_x)
    df_test.loc[:, 'Prelabel_Multi_A'] = y_predict
    df_test.to_csv(filename, index=None)
    
    metricfile = testfile + '_Metrics_ATC.csv'
    metrics_list = [testfile]
    Precision = precision_score(test_y, y_predict, average='weighted', zero_division=0)
    Recall = recall_score(test_y, y_predict, average='weighted', zero_division=0)
    F1 = f1_score(test_y, y_predict, average='weighted', zero_division=0)
    Accuracy = accuracy_score(test_y, y_predict)
    
    confusion = confusion_matrix(test_y, y_predict)
    FP = confusion.sum(axis=0) - np.diag(confusion)  
    FN = confusion.sum(axis=1) - np.diag(confusion)
    TP = np.diag(confusion)
    TN = confusion.sum() - (FP + FN + TP)
    
    FPR = FP / (FP + TN)
    FNR = FN / (FN + TP)
    
    metrics_list.extend([str(Precision), str(Recall), str(F1), str(Accuracy), str(np.mean(FPR)), str(np.mean(FNR)), str(detection_time)])
    
    print(metrics_list)
    print('Recall:', Recall)
    print('Precision:', Precision)
    print('F1 Score:', F1)
    print('Accuracy:', Accuracy)
    print('Average FPR:', np.mean(FPR))
    print('Average FNR:', np.mean(FNR))
    print('Detection Time:', detection_time, 'seconds')
    write_metrics(metricfile, metrics_list)
    
    if not df_test.empty:
        report = classification_report(test_y, y_predict, target_names=test_y['Label_Multi_A'].unique().tolist(), output_dict=True)
        for key, value in report.items():
            if isinstance(value, dict):
                print(key)
                for k, v in value.items():
                    if k != 'support':
                        # Check if the DataFrame is not empty before accessing its elements
                        if not df_test[df_test['Label_Multi_A'] == key].empty:
                            print(f"{k}: {v:.2f} \t{df_test[df_test['Label_Multi_A'] == key]['Label_Multi_A'].values[0]}: {df_test[df_test['Label_Multi_A'] == key]['Detection_Time'].mean():.2f} seconds")
            else:
                print(f"{key}: {value}")
    else:
        print("DataFrame is empty, classification report cannot be generated.")
    
    plt.figure(figsize=(15, 8))
    plt.imshow(confusion, cmap=plt.cm.YlOrBr)
    indices = range(len(confusion))
    classes = ['Benign', 'Slowloris', 'RUDY', 'Slow READ', 'COMM', 'Slow NEXT', 'RANGE', 'Slow DROP', 'REQ']
    plt.colorbar()
    plt.title('Model: ' + alg + '     F1-Score: ' + str(F1))
    plt.xticks(indices, classes)
    plt.yticks(indices, classes)
    
    for first_index in range(len(confusion)):
        for second_index in range(len(confusion[first_index])):
            plt.text(first_index, second_index, confusion[second_index][first_index])
    
    plt.xlabel('Predicted')
    plt.ylabel('Actual')
    plt.savefig(alg + ' ATC Confusion.pdf')

def dtct():
    detect(trainFile, testFile, out256, 'RF')
    detect(trainFile, testFile, out256, 'SVM')  
    detect(trainFile, testFile, out256, 'DT')  

if __name__ == '__main__':
    dtct()
