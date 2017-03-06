from application import application
import random,hashlib

if __name__ == "__main__":
    application.secret_key = hashlib.sha256(str(random.getrandbits(256)).encode('utf-8')).hexdigest()
    application.run()
