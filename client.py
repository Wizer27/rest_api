import requests as r

def pos_request():
    url = "http://localhost:8080/api/data"
    text_data = "Это просто обычный текст без JSON"
    headers = {"Content-Type": "text/plain"}

    response = r.post(url, data=text_data, headers=headers)

    print("Status code:", response.status_code)
    print("Response:", response.text)


def get_request():
    url = "http://localhost:8080/api/view"

    try:
        response = r.get(url)
        try:
            print("####### STATUS CODE ####### ")
            print(response.status_code)
        except Exception as e:
            print(f"No staus code :{e}")    
        return response.text
    except Exception as e:
        print(f"Error {e}")
        return ""
    

print(get_request())
