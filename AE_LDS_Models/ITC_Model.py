from sklearn.metrics import classification_report
import pandas as pd
from sklearn.metrics import recall_score, precision_score, f1_score
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
from sklearn import ensemble
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
import csv

# specify training set file and testing set file
trainFile = "Train_Set.csv"
testFile = "MixedTraffic.csv"
metricfile = ''

# -------------write the predicted labels to the file-------------
def writeprelabel(filename, prelabel):
    df = pd.read_csv(filename)
    df['Prelabel_T'] = prelabel
    df.to_csv(filename, index=None)

# -------------write the evaluation indicators to the file-------------
def write_metrics(metricfile, metrics_list):
    with open(metricfile, 'a', newline='') as student_file:
        writer = csv.writer(student_file)
        writer.writerow(metrics_list)

def detect(trainfile, testfile, alg):
    # Load and prepare training data
    data = pd.read_csv(trainfile)
    train_y = data[['Label_T']]
    train_x = data[['F_Pck', 'B_Pck', 'F_P_L1', 'B_P_L1', 'F_TCP_S_C', 'B_TCP_S_C', 'A_M', 'I_M']]
    
    # Load and prepare test data
    data_test = pd.read_csv(testfile)
    test_y = data_test[['Label_T']]
    test_x = data_test[['F_Pck', 'B_Pck', 'F_P_L1', 'B_P_L1', 'F_TCP_S_C', 'B_TCP_S_C', 'A_M', 'I_M']]

    # Standardize features by removing the mean and scaling to unit variance
    scaler = StandardScaler()
    train_x_scaled = scaler.fit_transform(train_x)
    test_x_scaled = scaler.transform(test_x)

    if alg == 'DT':
        clf = DecisionTreeClassifier(criterion='entropy', max_depth=4, min_samples_leaf=15, splitter='random', random_state=None)
        print('alg: DT')
    elif alg == 'RF':
        clf = ensemble.RandomForestClassifier(n_estimators=8)
        print('alg: RF')
    elif alg == 'SVM':
        clf = SVC(kernel='linear')  # Adjusted to handle standardized data
        print('alg: SVM')

    # Fit the model
    clf = clf.fit(train_x_scaled, train_y.values.ravel())
    y_predict = clf.predict(test_x_scaled)
    writeprelabel(testfile, y_predict)

    # Evaluation
    metricfile = testfile + '_Metrics_ITC.csv'
    metrics_list = [testfile]
    precision = precision_score(test_y, y_predict)
    recall = recall_score(test_y, y_predict)
    F1 = f1_score(test_y, y_predict)

    metrics_list.extend([str(precision), str(recall), str(F1)])
    print('recall:', recall)
    print('precision:', precision)
    print('F1 score:', F1)
    write_metrics(metricfile, metrics_list)

    # Confusion matrix visualization
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
    plt.savefig(alg + ' ITC Confusion.pdf')

# -------------select the algorithm: DT, RF, SVM-------------
def dtct():
    detect(trainFile, testFile, 'DT')
    detect(trainFile, testFile, 'RF')
    detect(trainFile, testFile, 'SVM')

if __name__ == '__main__':
    dtct()
