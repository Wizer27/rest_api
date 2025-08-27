from fastapi import FastAPI,HTTPException
import json

app = FastAPI()

@app.get("/")
async def main():
    return "AI urist API"