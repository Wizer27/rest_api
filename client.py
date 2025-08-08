import requests as r;


url = "http://localhost:8080/api/data"
text_data = "Это просто обычный текст без JSON"
headers = {"Content-Type": "text/plain"}

response = r.post(url, data=text_data, headers=headers)

print("Status code:", response.status_code)
print("Response:", response.text)
