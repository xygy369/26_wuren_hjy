from launch import LaunchDescription   #导入launchdescription  ,作为容器 
from launch_ros.actions import Node    #node描述启动的节点
import os     #拼接文件路径   上次第二次培训用到
from ament_index_python.packages import get_package_share_directory  #查找功能包安装后路径，拷贝到install
def generate_launch_description():    
    # 拼出 YAML 文件路径
    config = os.path.join(
        get_package_share_directory('ted_8'),  # 功能包名        
        'config',                              # 文件夹名        
        'ted_8.yaml'                            # YAML文件名
    )    
    # 节点1：ted
    turtlesim_node = Node(        
        package='turtlesim',       # 小海龟的包名        
        executable='turtlesim_node',   # 可执行文件名
    )    
    # 节点2：8
    ted_8_node = Node (        
        package='ted_8',       # 你的功能包名        
        executable='ted_8_node',   # 你的可执行文件名        
        parameters=[config],   # YAML路径变量
    )    
    return LaunchDescription([
        turtlesim_node,
        ted_8_node,
    ])