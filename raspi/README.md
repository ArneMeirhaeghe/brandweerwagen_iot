//voor te starten --> op de pi natuurlijk

//als er geen venv map is
python -m venv venv 

//vnv activeren
source venv/bin/activate
sudo systemctl restart mosquitto --> moet niet maar kan als het niet werkt 
pip install -r requirements.txt

python3 app.py --> in de map van raspii
