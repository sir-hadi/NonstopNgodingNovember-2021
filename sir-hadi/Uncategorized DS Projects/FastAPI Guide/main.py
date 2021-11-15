# Import Library
import pickle
from fastapi import FastAPI
from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware
import minisom

# Confiigure CORS and Create FastAPI Object
app = FastAPI()

origins = ["*"]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

#Encode Classes
sex_class = pickle.load(open('se_class.p','rb'))
em_class = pickle.load(open('em_class.p','rb'))
model = pickle.load(open('model.p','rb'))

class Titanic(BaseModel):
	Pclass: float
	Sex: str
	Age: float
	SibSp: float
	Parch: float
	Fare: float
	Embarked: str

#Function For encoding
def encode_sex(x):
    return sex_class.index(x)

def encode_em(x):
    return em_class.index(x)


@app.get("/predict")
def predict(data: Titanic):
    arr = [data.Pclass,
           encode_sex(data.Sex),
           data.Age,
           data.SibSp,
           data.Parch,
           data.Fare,
           encode_em(data.Embarked)
        ]
    result = model.predict([arr])
	
    if (result == 0):
        return { "Status" : "Dead"}
    else:
        return {"Status" : "Survive"}