import time
import serial
import serial.tools.list_ports
import threading
import cv2
from roboflow import Roboflow
rf = Roboflow(api_key="7Acu0BavRYmFhtgXrJqz")
project = rf.workspace().project("smoke100-uwe4t")
model = project.version(5).model
webcam = cv2.VideoCapture(0)

def send_vr():
    sendData = f"VR=?\n"
    my_serial.write( sendData.encode() )
def send_ambient_temperature():
    sendData = f"AMBIENT=?\n"
    my_serial.write( sendData.encode() )
def send_dc(digree):
    sendData = f"DC={digree}\n"
    my_serial.write( sendData.encode() )
def send_buzzer(freq):
    sendData = f"BUZZER={freq}\n"
    my_serial.write( sendData.encode() )
def send_fnd(data):
    sendData = f"FND={data}\n"
    my_serial.write( sendData.encode() )
def send_rgb_led(red,green,blue):
    sendData = f"RGB={red},{green},{blue}\n"
    my_serial.write( sendData.encode() )

serial_receive_data = ""
def serial_read_thread():
    global serial_receive_data
    while True:
        read_data = my_serial.readline()
        serial_receive_data = read_data.decode()

def process(data):
    vr = data[0]
    temp = data[1]
    print("저항 : {} | 온도 : {}".format(vr, temp))
    if 0 <= vr < 5:
        if temp <= 30:
            send_fnd(temp)
            send_rgb_led(0,0,255)
            send_dc(0)
        elif 30 < temp <= 40:
            send_fnd(temp)
            send_rgb_led(0,255,0)
            send_dc(1)
        elif 40 < temp <= 50:
            send_fnd(temp)
            send_rgb_led(255,255,0)
            send_dc(2)
        elif 50 < temp <= 80:
            send_fnd(temp)
            send_rgb_led(255,50,0)
            send_dc(3)
        elif temp > 80:
            send_fnd(temp)
            send_rgb_led(255,0,0)
            send_dc(4)
            send_buzzer(300)
            time.sleep(0.5)
            send_buzzer(0)
            print("부저 작동")
    else:
        if vr < 300:
            speed = 1
        elif 300 <= vr < 500:
            speed = 2
        elif 500 <= vr < 700:
            speed = 3
        elif vr >= 700:
            speed = 4
        send_fnd(temp)
        send_rgb_led(255,255,255)
        send_dc(speed)

receive_list = []

def vr_temp(a):
    global receive_list
    receive_list.append(a)
    if len(receive_list) == 2:
        process(receive_list)
        receive_list = []

def main():
    try:
        global serial_receive_data
        time_num= 0
        curr_time = 0
        prev_time = 0
        smk = 1
        #-- 웹캠 열기
        while webcam.isOpened():
            
            status, frame = webcam.read()
            time_num = time_num + 1
            
            if not status:
                break

            #-- 출력창
            cv2.imshow("WebCam", frame)
            
            # -- 캡쳐 프레임 간격 설정
            if time_num%30 == 0.0:
                
                a = model.predict(frame, confidence=30, overlap=30).json()
                
                if len(a['predictions']) == 0:
                    smk = 1
                else:
                    print("연기 감지!")
                    smk = 0
                    smoke_data = [0, 100]
                    process(smoke_data)
                    send_buzzer(300)
                    time.sleep(0.5)
                    send_buzzer(0)
                    send_buzzer(300)
                    time.sleep(0.5)
                    send_buzzer(0)
                    send_buzzer(300)
                    time.sleep(0.5)
                    send_buzzer(0)
                    

            if smk == 1:
                if "VR=" in serial_receive_data:
                    vr = int(serial_receive_data[3:])
                    vr_temp(vr)
                    serial_receive_data = ""
                elif "AMBIENT=" in serial_receive_data:
                    temp = float(serial_receive_data[8:])
                    vr_temp(temp)
                    serial_receive_data = ""
                
                curr_time = time.time()
                if curr_time - prev_time >= 1.0:
                    prev_time = curr_time
                    send_vr()
                    time.sleep(0.5)
                    send_ambient_temperature()

            #-- 0 입력시 종료 
            if cv2.waitKey(1) & 0xFF == ord('0'):
                break
                    
        webcam.release()
        cv2.destroyAllWindows()

    except KeyboardInterrupt:
        pass

if __name__ == '__main__':
    
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if 'Arduino Uno' in p.description:
            print(f"{p} 포트에 연결하였습니다.")
            my_serial = serial.Serial(p.device, baudrate=9600, timeout=1.0)
            time.sleep(2.0)

    t1 = threading.Thread(target=serial_read_thread)
    t1.daemon = True
    t1.start()
    
    main()
    
    my_serial.close()