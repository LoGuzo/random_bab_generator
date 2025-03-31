import os
import subprocess

from dotenv import load_dotenv
from slack_bolt import App
from slack_bolt.adapter.socket_mode import SocketModeHandler
from slack_sdk import WebClient

# pip install slack-sdk slack-bolt python-dotenv

# Load environment variables from .env (if present)
load_dotenv()

# Securely load tokens from environment variables
APP_TOKEN = os.getenv("SLACK_APP_TOKEN")  # Use 'xapp-' token
BOT_TOKEN = os.getenv("SLACK_BOT_TOKEN")  # Use 'xoxb-' token

if not APP_TOKEN or not BOT_TOKEN:
    raise ValueError(
        "Missing SLACK_APP_TOKEN or SLACK_BOT_TOKEN. Set them in .env or environment variables."
    )

# Initialize Slack Bolt App
app = App(token=BOT_TOKEN)
client = WebClient(token=BOT_TOKEN)


# Function to execute your C program and get results
def run_c_program(channel_id):
    try:
        build_directory = os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "..", "build"
        )

        # Run the program from the 'build' directory
        result = subprocess.run(
            [os.path.join(build_directory, "Random_Bab"), channel_id],
            capture_output=True,
            text=True,
        )

        if result.returncode == 0:
            return result.stdout.strip()
        else:
            return f"Error generating groups: {result.stderr.strip()}"
    except Exception as e:
        return f"An error occurred: {str(e)}"


# Function to check if the user is a manager
def is_manager(user_id):
    try:
        user_info = client.users_info(user=user_id)
        if user_info["ok"]:
            real_name = user_info["user"].get("real_name", "").lower()
            display_name = (
                user_info["user"].get("profile", {}).get("display_name", "").lower()
            )
            title = user_info["user"].get("profile", {}).get("title", "").lower()

            # Check if any of these fields contain "manager" or "매니저"
            # keywords = ["manager", "매니저"]
            # keywords = ["admin"]
            keywords = ["조"]
            if any(
                keyword in real_name or keyword in display_name or keyword in title
                for keyword in keywords
            ):
                return True
    except Exception as e:
        print(f"Error fetching user info: {e}")

    return False  # Default to no access


# Slash Command Handler
@app.command("/randbab")
def custom_command_function(ack, respond, command):
    ack()  # Acknowledge Slack request

    user_id = command["user_id"]
    channel_id = command["channel_id"]  # Get the channel where the command was used

    # Check if the user is a manager
    if not is_manager(user_id):
        respond(
            "🚫 You are not authorized to use this command. Only managers can execute this action."
        )
        return

    # Run the C program if authorized
    output = run_c_program(channel_id)

    # Send the message to the whole channel
    client.chat_postMessage(channel=channel_id, text="<!channel>")
    formatted_message = f"👥 *Group Generation Result:*\n```{output}```"
    client.chat_postMessage(
        channel=channel_id, text=formatted_message
    )  # Send to channel
    client.chat_postMessage(channel=channel_id, text="End Chat\n")


# Start Slack Bot in Socket Mode
def main():
    handler = SocketModeHandler(app, APP_TOKEN)
    handler.start()


if __name__ == "__main__":
    main()
