from sklearn import datasets   #自带的花数据集
from sklearn.model_selection import train_test_split    #划分训练集和测试集
from sklearn.svm import SVC         #SVC分类器
from sklearn.metrics import accuracy_score, classification_report     #评价指标
import matplotlib      #画图工具
matplotlib.use('TkAgg')      #指定窗口显示照片
import matplotlib.pyplot as plt     #画图库接口

# 加载数据
iris = datasets.load_iris()             #花数据集   150数据   4特征   3类别
X = iris.data          #特征矩阵    150行   4列
y = iris.target           #标签代表三种花

print("=" * 40)
print("         鸢尾花数据集")
print("=" * 40)
print(f"数据量: {len(X)} 条")
print(f"特征数: {X.shape[1]} 个")
print(f"三个类别: {list(iris.target_names)}")
print(f"四个特征: {list(iris.feature_names)}")
print()
print("前5条数据:")
for i in range(5):
    print(f"  {X[i]} -> {iris.target_names[y[i]]}")
print()

# 划分训练集/测试集
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.3, random_state=42
)      #0.7  训练    0.3测试
print(f"训练集: {len(X_train)} 条")       
print(f"测试集: {len(X_test)} 条")
print()

# 训练SVM
#支持向量 决定分界线位置         间隔 分界线到两边最近的点    核函数   不是线性可分时，升维 在高维里用平面分开 
model = SVC(kernel='rbf')             #svm分类器 支持向量机找到分界线   rbf是核函数类型
model.fit(X_train, y_train)        #将训练数据喂给模型
print("训练完成!")
print()

# 预测
y_pred = model.predict(X_test)    #测试集喂给训练的模型  做预测

print("真实值 vs 预测值:")
print(f"  {'真实':>12s}  {'预测':>12s}  结果")
print("  " + "-" * 30)
for i in range(len(y_test)):
    t = iris.target_names[y_test[i]]
    p = iris.target_names[y_pred[i]]
    ok = "OK" if y_test[i] == y_pred[i] else "X"
    print(f"  {t:>12s}  {p:>12s}  {ok}")
print()

# 准确率
acc = accuracy_score(y_test, y_pred)
print("=" * 40)
print(f"  准确率: {acc:.2%}")
print("=" * 40)
print()

# 详细报告
print("分类报告:")
print(classification_report(y_test, y_pred, target_names=iris.target_names))

# 不同核函数对比
print("=" * 40)
print("  不同核函数对比")
print("=" * 40)
for k in ['linear', 'rbf', 'poly']:     #直线  曲线 多项式曲线
    m = SVC(kernel=k)
    m.fit(X_train, y_train)
    a = accuracy_score(y_test, m.predict(X_test))
    bar = "#" * int(a * 20)
    print(f"  {k:>10s}  准确率:{a:.2%}  {bar}")
print()

# 画图
fig, axes = plt.subplots(1, 2, figsize=(12, 5))
colors = ['red', 'green', 'blue']
names = list(iris.target_names)

for i in range(3):
    mask = y_test == i
    axes[0].scatter(X_test[mask, 0], X_test[mask, 1],
                    c=colors[i], label=names[i], s=80, edgecolors='black')
axes[0].set_xlabel('sepal length')
axes[0].set_ylabel('sepal width')
axes[0].set_title('True Label')
axes[0].legend()
axes[0].grid(True)

for i in range(3):
    mask = y_pred == i
    axes[1].scatter(X_test[mask, 0], X_test[mask, 1],
                    c=colors[i], label=names[i], s=80, edgecolors='black')
axes[1].set_xlabel('sepal length')
axes[1].set_ylabel('sepal width')
axes[1].set_title('SVM Prediction')
axes[1].legend()
axes[1].grid(True)

plt.tight_layout()
plt.savefig('svm_result.png', dpi=150)
print("图片已保存: svm_result.png")
plt.show()

"""
加载数据 → 拆成训练集和测试集 → 用训练集训练SVM → 用测试集检验 → 打印准确率 → 画图对比
"""