import pyrebase

firebaseConfig = {
  'apiKey': "AIzaSyBlh1_rlUcP6Q6K8vDf4luiEkoa4adEe-s", 
  'authDomain': "testapp-e0800.firebaseapp.com",
  'projectId': "testapp-e0800",
  'storageBucket': "testapp-e0800.appspot.com",
  'messagingSenderId': "194888033602",
  'appId': "1:194888033602:web:88b8c689ab604d8bad14db",
  'measurementId': "G-GD6BTPC071",
  'databaseURL': "https://testapp-e0800-default-rtdb.europe-west1.firebasedatabase.app"}


#apiKey: The API key of the Firebase project, used to authenticate the app with Firebase services.
#authDomain: The Firebase project's authentication domain.
#projectId: The ID of the Firebase project.
#storageBucket: The name of the Firebase project's Cloud Storage bucket.
#messagingSenderId: The sender ID for Firebase Cloud Messaging.
#appId: The unique identifier of the Firebase project's web app.
#measurementId: The Google Analytics measurement ID of the Firebase project.
#databaseURL: The URL of the Firebase project's Realtime Database.



def init_database(productID):
  firebase = pyrebase.initialize_app(firebaseConfig)
  db = firebase.database()
  data = {"stream flag": 0, "notification_flag": 0, "new_audio":0}

  db.child(productID).child().set(data)

  return 1


def get_data(productID, data_child):
  firebase = pyrebase.initialize_app(firebaseConfig)
  db = firebase.database()
  data = db.child(productID).child(data_child).get()
  value = int(data.val())
  
  if(value == None):
    return -1
  
  return value

def push_data(productID, data_child, data_value):
  firebase = pyrebase.initialize_app(firebaseConfig)
  db = firebase.database()
  db.child(productID).update({data_child:data_value})
  
  return 1
  
  
def upload_video(productID,video_file):
  firebase = pyrebase.initialize_app(firebaseConfig)
  path = "/etc/doorbell/" + video_file
  storage = firebase.storage()
  
  storage.child(productID + '/' + video_file).put(path)
   
  return 1
  











 
