import json

def contains_username(username:str) -> bool:
    with open("/Users/ivan/rest_api/data/videos.json","r") as file:
        data = json.load(file)


    for user in data:
        if user["username"] == username:
            return True

    return False        

def base_videos_data(username:str) -> bool:
    with open("/Users/ivan/rest_api/data/videos.json","r") as file:
        data = json.load(file)

    if not contains_username(username):
        data.append({
            "username":username,
            "videos":{}
        }) 
        with open("/Users/ivan/rest_api/data/videos.json","w") as file:
            json.dump(data,file,indent=2)
            return True
    else:
        print("User already exists")
        return False

base_videos_data("ivan")        