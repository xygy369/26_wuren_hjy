import torch.nn as nn

#卷积神经网络
class ConeCNN(nn.Module):
    def __init__(self, num_classes=3):   #三种类别
        super(ConeCNN, self).__init__()
        self.conv1 = nn.Conv2d(3, 16, kernel_size=3, padding=1) #卷积层1 rgb3通道 16个特征 卷积核3*3 尺寸不变
        self.bn1 = nn.BatchNorm2d(16) #归一化 每个数字都转化为方差为1  均值为0的标准正态分布 跟上次的多元线形回归有点像
        self.pool1 = nn.MaxPool2d(2, 2) #池化层 窗口2*2  步长2  保留最强的特征  减少计算量
        self.conv2 = nn.Conv2d(16, 32, kernel_size=3, padding=1)  #卷积层2 上一层的16特征 输出32特征
        self.bn2 = nn.BatchNorm2d(32)
        self.pool2 = nn.MaxPool2d(2, 2)
        self.conv3 = nn.Conv2d(32, 64, kernel_size=3, padding=1) #卷积层3
        self.bn3 = nn.BatchNorm2d(64)
        self.pool3 = nn.MaxPool2d(2, 2)
        self.flatten = nn.Flatten() #展平层 将多维数据换成一维   从而传给全连接层
        self.fc1 = nn.Linear(64 * 8 * 8, 128)  #全连接层1  输入展平后的64*8*8 输出128神经元
        self.dropout = nn.Dropout(0.5) #ai说随机丢弃（并非删除，只是训练时不工作）一半神经元  防止过拟合  训练时开
        self.fc2 = nn.Linear(128, num_classes)  #输出3个类别
        self.relu = nn.ReLU()  #激活函数  负数变0 正数不变 引入非线性

    def forward(self, x):  #定义数据流动方向 从输出到输入
        x = self.pool1(self.relu(self.bn1(self.conv1(x)))) #卷积块1
        x = self.pool2(self.relu(self.bn2(self.conv2(x))))
        x = self.pool3(self.relu(self.bn3(self.conv3(x))))
        x = self.flatten(x) #展平
        x = self.relu(self.fc1(x))
        x = self.dropout(x)
        x = self.fc2(x) #输出
        return x  #最后放回三个数 对应每个颜色概率
