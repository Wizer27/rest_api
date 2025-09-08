from fastapi import FastAPI,HTTPException, status
from pydantic import BaseModel
import json
import threading
import socket
from typing import Union,Literal

from pydantic.types import StrictStr
from pydantic_core.core_schema import str_schema


app = FastAPI()


@app.get("/")
async def main():
    return "Alexandria videos API"


def contains_username(username:str) -> bool:
    with open("/Users/ivan/rest_api/data/videos.json","r") as file:
        data = json.load(file)


    for user in data:
        if user["username"] == username:
            return True

    return False


def has_key(key : str,data) -> bool:
    return key in data

def write_def_postst(username:str) -> bool:
    with open("/Users/ivan/rest_api/data/posts.json","r") as file:
        main = json.load(file)

    for user in main:
        if user["username"] == username:
            print("Here 1")
            return False
    main.append({
        "username":username,
        "posts":[]
    })
    with open("/Users/ivan/rest_api/data/posts.json","w") as file:
        json.dump(main,file,indent=2)

    return True

def fix_enumaration(username:str,title:str,code:str):
    with open("/Users/ivan/rest_api/data/videos.json","r") as file:
        data = json.load(file)
    ok = False
    if contains_username(data):
        for user in data:
            if has_key(title,user["videos"]):
                user["videos"][title + "1"] = code
                ok = True
            else:
                print("Key doest exist")
    else:
        print("Unvalid username")

    if ok:
        with open("/Users/ivan/rest_api/data/videos.json","w") as file:
            json.dump(data,file,indent=2)
    else:
        print("Error")


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


class Username(BaseModel):
    username :str


@app.post("/load/default/videos")
async def load_def(request:Username):
    if base_videos_data(request.username):
        return "OK"
    raise HTTPException(status_code=404,detail="User already exists")


class JsonDataUser(BaseModel):
    username:str
    code:str
    title:str

@app.post("/write/video/user")

async def write_video_to_user(request:JsonDataUser):
    with open("/Users/ivan/rest_api/data/videos.json","r") as file:
        data = json.load(file)
    for user in data:
        if user["username"]  == request.username:
            try:
                user["videos"][request.title] = request.code
            except Exception as e:
                print(f"Expceptin : {e}")
    with open("/Users/ivan/rest_api/data/videos.json","w") as file:
        json.dump(data,file,indent=2)
        print("Success")

class Request_Video_Data(BaseModel):
    username:str
    title:str


@app.post("/get/video")
async def get_video(request:Request_Video_Data):
    with open("/Users/ivan/rest_api/data/videos.json","r") as file:
        data = json.load(file)

    for user in data:
        if user["username"] == request.username:
            if has_key(request.title,user["videos"]):
                return user["videos"][request.title]
            else:
                return "No such title"
    return "No such user"


class DeleteVideo(BaseModel):
    username:str
    title:str

@app.post("/delete/video")
async def delete_video(request:DeleteVideo):
    with open("/Users/ivan/rest_api/data/videos.json","r") as file:
        data = json.load(file)
    global ex
    ex = False
    for user in data:
        if user["username"] == request.username:
            if has_key(request.title,user["videos"]):
                del user["videos"][request.title]
                ex = True
            else:
                raise HTTPException(status_code=400,detail="Video not found")
    if ex:
        with open("/Users/ivan/rest_api/data/videos.json","w") as file:
            json.dump(data,file,indent=2)


class LikedPost(BaseModel):
    username:str
    author:str
    title:str
    post:str

@app.post("/liked/posts")
async def write_liked_post(request:LikedPost):
    with open("/Users/ivan/rest_api/data/liked_posts.json","r") as file:
        data = json.load(file)
    ok = False
    for user in data:
        if user["username"] == request.username:
            user["posts"].append({
                "author":request.author,
                "title":request.title,
                "post":request.post
            })
            ok = True
    if ok:
        with open("/Users/ivan/rest_api/data/liked_posts.json","w") as file:
            json.dump(data,file,indent=2)
    else:
        raise HTTPException(status_code=400,detail="user not found")


class DefRequestLiked(BaseModel):
    username:str

@app.post("/liked/default")
async def write_def(request:DefRequestLiked):
    with open("/Users/ivan/rest_api/data/liked_posts.json","r") as file:
        data = json.load(file)
    for user in data:
        if user["username"] == request.username:
            raise HTTPException(status_code=400,detail="User alredy exists")

    data.append(
        {
            "username":request.username,
            "posts":[]
        }
    )
    with open("/Users/ivan/rest_api/data/liked_posts.json","w") as file:
        json.dump(data,file,indent=2)

class DeleteRequest(BaseModel):
    username:str
    author:str
    title:str
    post:str

@app.post("/liked/delete")
def delte_post_from_liked(request:DeleteRequest):
    with open("/Users/ivan/rest_api/data/liked_posts.json","r") as file:
        data = json.load(file)
    global ok
    ok = False
    for user in data:
        if user["username"] == request.username:
            for post in user["posts"]:
                if post["title"] == request.title and post["author"] == request.author and post["post"] == request.post:
                    index = user["posts"].index(post)
                    user["posts"].pop(index)
                    ok = True
    if ok:
        with open("/Users/ivan/rest_api/data/liked_posts.json","w") as file:
            json.dump(data,file,indent=2)
    else:
        raise HTTPException(status_code=400,detail="User not found")

class ShowLiked(BaseModel):
    username:str
@app.post("/get/liked")
async def get_liked(request:ShowLiked):
    with open("/Users/ivan/rest_api/data/liked_posts.json","r") as file:
        data = json.load(file)
    for user in data:
        if user["username"] == request.username:
            return user["posts"]

    raise HTTPException(status_code=400,detail="User not found")





class Register(BaseModel):
    username:str
    hash_psw:str
@app.post("/register")
async def register(request:Register):
    with open("/Users/ivan/rest_api/data/users.json","r") as file:
        data = json.load(file)

    with open("/Users/ivan/rest_api/data/subs.json","r") as file:
        subs = json.load(file)

    if not has_key(request.username,data):
        data[request.username] = request.hash_psw
        with open("/Users/ivan/rest_api/data/users.json","w") as file:
            json.dump(data,file,indent=2)
    else:
        raise HTTPException(status_code=400,detail="User alredy exist")

    if request.username in subs:
        raise HTTPException(status_code = 400,detail = "Eror with the subs")
    else:
        subs[request.username] = []
        with open("/Users/ivan/rest_api/data/subs.json","w") as file:
            json.dump(subs,file,indent=2)

@app.post("/login")
async def login(request:Register):
    with open("/Users/ivan/rest_api/data/users.json","r") as file:
        data = json.load(file)
    if has_key(request.username,data):
        if data[request.username] == request.hash_psw:
            return True
        else:
            raise HTTPException(status_code=400,detail="Wrong password")
    else:
        raise HTTPException(status_code=404,detail="User not found")

class Write_Default_Posts(BaseModel):
    username:str

@app.post("/write_default/posts")
async def write_default_posts(request:Write_Default_Posts):
    username = request.username
    if write_def_postst(username):
        return True
    raise HTTPException(status_code=400,detail = "Something went wrong")

class Post(BaseModel):
    author:str
    title:str
    content:str

class VideoPost(Post):
    type: Literal["video"] = "video"
    video:str

class PhotoPost(Post):
    type: Literal["photo"] = "photo"
    photo:str

posts = Union[VideoPost, PhotoPost, Post]
@app.post("/write/post")
async def write_post_to_user(request:posts):
    with open("/Users/ivan/rest_api/data/posts.json","r") as file:
        data = json.load(file)

    base = {
        "author":request.author,
        "title":request.title,
        "content":request.content,
        "type":"text"
    }
    if isinstance(request,VideoPost):
        base["video"] = request.video
        base["type"] = "video"
    elif isinstance(request,PhotoPost):
        base["photo"] = request.photo
        base["type"] = "photo"

    user_ex = False
    for user in data:
        if user["username"] == request.author:
            user_ex = True
    if user_ex:
        for user2 in data:
            if user2["username"] == request.author:
                user2["posts"].append(base)
        with open("/Users/ivan/rest_api/data/posts.json","w") as file:
            json.dump(data,file,indent=2)

        with open("/Users/ivan/rest_api/data/likes.json","r") as file:
            likes = json.load(file)

        likes.append({
            "author":request.author,
            "titile":request.title,
            "likes":[],
            "dislikes":[]
        })
        with open("/Users/ivan/rest_api/data/likes.json","w") as file:
            json.dump(likes,file,indent=2)
    else:
        raise HTTPException(status_code=400,detail="User not found")

@app.post("/delete/post")
async def delete_post(request:posts):
    with open("/Users/ivan/rest_api/data/posts.json","r") as file:
        data = json.load(file)

    base_search = {
        "author":request.author,
        "title":request.title,
        "content":request.content,
        "type":"text"
    }
    if isinstance(request,VideoPost):
        base_search["video"] = request.video
        base_search["type"] = "video"
    elif isinstance(request,PhotoPost):
        base_search["photo"] = request.photo
        base_search["type"] = "photo"

    user_ex = False
    post_deleted = False
    for i in data:
        if i["username"] == request.author:
            user_ex = True
    if user_ex:
        try:
            main_type = base_search["type"]
            for user in data:
                for post in user["posts"]:
                    if main_type == "text" and base_search["author"] == post["author"] and base_search["title"] == post["title"] and base_search["content"] == post["content"]:
                        inex = user["posts"].index(post)
                        user["posts"].pop(inex)
                        post_deleted = True
                        break
                    elif main_type == "video" and base_search["author"] == post["author"] and base_search["title"] == post["title"] and base_search["content"] == post["content"] and base_search["video"] == post["video"]:
                        inex = user["posts"].index(post)
                        user["posts"].pop(inex)
                        post_deleted = True
                        break
                    elif main_type == "photo" and base_search["author"] == post["author"] and base_search["title"] == post["title"] and base_search["content"] == post["content"] and base_search["photo"] == post["photo"]:
                        inex = user["posts"].index(post)
                        user["posts"].pop(inex)
                        post_deleted = True
                        break
        except Exception as e:
            raise HTTPException(status_code=404,detail= f"Error : {e}")
    else:
        raise HTTPException(status_code=400,detail="User not found")
    if post_deleted:
        with open("/Users/ivan/rest_api/data/posts.json","w") as file:
            json.dump(data,file)
    else:
        raise HTTPException(status_code=400,detail="Something went wrong")

class GetPosts(BaseModel):
    username:str

@app.post("/get/user/posts")
def get_user_posts(request:GetPosts):
    with open("/Users/ivan/rest_api/data/posts.json","r") as file:
        data = json.load(file)
    for user in data:
        if user["username"] == request.username:
            return user["posts"]
    raise HTTPException(status_code=404,detail="User not found")    
        


class SudoLike(BaseModel):
    username:str
    title:str
    author:str
@app.post("/like/post")
async def like_sudo_post(request:SudoLike):
    with open("/Users/ivan/rest_api/data/likes.json","r") as file:
        data = json.load(file)

    user_ex = False
    for i in data:
        if i["author"] == request.author:
            user_ex = True
    if user_ex:
        for user in data:
            if user["author"] == request.author and user["title"] == request.title and request.username not in user["likes"] and request.username not in user["dislikes"]:
                user["likes"].append(request.username)
            elif user["author"] == request.author and user["title"] == request.title and request.username not in user["likes"] and request.username in user["dislikes"]:
                ind = user["dislikes"].index(request.username)
                user["dislikes"].pop(ind)
                user["likes"].append(request.username)
            elif user["author"] == user.author and user["title"] == request.title and request.username in user["likes"] and request.username not in user["dislikes"]:
                index = user["likes"].index(request.username)
                user["likes"].pop(index)
        with open("/Users/ivan/rest_api/data/likes.json","w") as file:
            json.dump(data,file)
    else:
        raise HTTPException(status_code=400,detail="User not found")

@app.post("/dislike/post")
async def dislike_post(request:SudoLike):
    with open("/Users/ivan/rest_api/data/likes.json","r") as file:
        data = json.load(file)

    user_ex = False
    for user in data:
        if user["author"] == request.author:
            user_ex = True
    if user_ex:
        for post in data:
            if post["author"] == request.author and post["title"] == request.title and request.username not in post["dislikes"] and request.username not in post["likes"]:
                post["dislikes"].append(request.username)
            elif post["author"] == request.author and post["title"] == request.title and request.username not in post["likes"] and request.username in post["dilikes"]:
                index = post["dislikes"].index(request.username)
                post["dislikes"].pop(index)
            elif post["author"] == request.author and post["title"] == request.title and request.username in post["likes"] and request.username not in post["dislikes"]:
                ind = post["likes"].index(request.username)
                post["likes"].pop(ind)
                post["dislikes"].append(request.username)
        with open("/Users/ivan/rest_api/data/likes.json","w") as file:
            json.dump(data,file)
    else:
        raise HTTPException(status_code = 400,detail = "User not found")
class Nano(BaseModel):
    username:str
    creator:str

@app.post("/user/sub")
async def sub(request:Nano):
    with open("/Users/ivan/rest_api/data/subs.json","r") as file:
        data = json.load(file)

    if request.creator in data:
        data[request.creator].append(request.username)
    else:
        raise HTTPException(status_code = 400,detail = "User not found")
    with open("/Users/ivan/rest_api/data/subs.json","w") as file:
        json.dump(data,file)

class GetSubs(BaseModel):
    username:str

@app.post("/get/subs")
async def get_subs(request:GetSubs):
    username = request.username
    with open("/Users/ivan/rest_api/data/subs.json","r") as file:
        data = json.load(file)

    if username in data:
        return data[username]
    else:
        raise HTTPException(status_code = 400,detail = "User not found")
async def unsub(request:Nano):
    with open("/Users/ivan/rest_api/data/subs.json","r") as file:
        data =  json.load(file)

    if request.creator in data:
        if request.username in data[request.creator]:
            ind = data[request.creator].index(request.username)
            data[request.creator].pop(ind)
            with open("/Users/ivan/rest_api/data/subs.json","w") as file:
                json.dump(data,file)
                return True
        else:
            raise HTTPException(status_code = 400,detail = "You are not subed")
    else:
        raise HTTPException(status_code = 400,detail = "User not found")
@app.post("/subs/count")
async def get_subs_count(request:GetSubs):
    with open("/Users/ivan/rest_api/data/subs.json","r") as file:
        data = json.load(file)
    if request.username in data:
        return len(data[request.username])
    raise HTTPException(status_code=404,detail="User not found")

