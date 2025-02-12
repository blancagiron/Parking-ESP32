# Authors: Blanca Girón, Raúl Martínez
# The script interacts with the Telegram API via a bot, retrieving updates and extracting the chat.id from each received message. This chat.id allows the bot to send messages to specific users or groups. The bot connects to Telegram using a token from BotFather, which authenticates the bot. Using the get_updates() method, the script fetches updates, extracts the chat.id from each message, and prints it to the console for easy viewing.

import asyncio
from telegram import Bot
async def main():
bot = Bot(token='*************') # Cambiar por tu token de telegram
updates = await bot.get_updates() # Usa 'await' para esperar la respuesta de Telegram
for update in updates:
print(update.message.chat.id)
# Ejecuta la función asincrónica
asyncio.run(main())
