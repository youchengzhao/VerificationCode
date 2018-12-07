'''
Created on 2018年11月5日

@author: Administrator
'''

from flask import Flask,request
import configparser
import json
import os 
import time

app = Flask(__name__)


class MySnowFlake:
    def __init__(self,workerId,datacenter):
        #self.start = time.mktime(time.strptime('2018-10-25 00:00:00', "%Y-%m-%d %H:%M:%S"))
        #self.start = int(self.start*100)
        self.start = 1540396800000
        self.last = int(time.time()*1000)
        self.worker_id = workerId
        self.datacenter= datacenter
        self.sequence = 0
    
    def next_id(self):
        stamp = int(time.time()*1000) 
        
        if stamp < self.last:
            pass
            #系统时间回退，抛出异常
        if stamp == self.last:
            self.sequence += 1
            if self.sequence == 0:
                stamp = int(time.time()*1000) 
        else :
            self.sequence = 0
    
        self.last = stamp
        x = ((stamp - self.start)<<22) | (self.datacenter << 17) | (self.worker_id <<12) | self.sequence
        return str(x)
            
        


#ALLOW_EXTENSIONS = set(['html', 'htm', 'doc', 'docx', 'mht', 'pdf'])
ALLOW_EXTENSIONS = set(['png','jpg' ])
def allowed_file(filename):
    return '.' in filename and \
            filename.rsplit('.', 1)[1] in ALLOW_EXTENSIONS

@app.route('/sf/v1.0',methods=['POST'])
def sf_yzm():
    state = {'state':False}
    try :
        file = request.files['file']
        print (file.filename)
        if file and allowed_file(file.filename):
            snow = MySnowFlake(2,0)
            file_name = snow.next_id() + file.filename
            #file_name = str(uuid.uuid4()) + '.' + filename.rsplit('.', 1)[1]
            file.save(os.path.join('img',file_name))
            message = os.popen('./main '+ 'img/'+file_name).readlines()
            str = ''
            for i in message:
                str += i
            return str 
    except Exception as e:
        print(e)
    
    return json.dumps(state,ensure_ascii=False)

if __name__ == '__main__':
    cf = configparser.ConfigParser()
    cf.read('test.conf')
    port = int(cf.get('http_server', 'http_server.port'))
    app.debug = True
    #app.run(host='192.168.0.22',port = port)
    app.run(host='localhost',port = port)
    pass
