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
def run_c_program(channel_id, token, group_size, names):
    try:
        build_directory = os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "..", "build"
        )

        # Construct the command arguments
        command_args = [os.path.join(build_directory, "Random_Bab"), channel_id, token]
        print(command_args)

        if group_size is not None:
            command_args.append(group_size)
        else:
            command_args.append("4")  # Default group size

        if names:
            command_args.extend(names)
        else:
            command_args.append("")  # Placeholder to indicate no names provided

        # Run the program with the arguments
        result = subprocess.run(command_args, capture_output=True, text=True)

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
            keywords = ["오"]
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
    command_text = command["text"].strip()  # Extract command text

    # Check if the user is a manager
    if not is_manager(user_id):
        respond(
            "🚫 You are not authorized to use this command. Only managers can execute this action."
        )
        return

    # Default values
    group_size = None
    names = []

    if command_text:  # If arguments are provided
        args = command_text.split()

        if args[0].isdigit():  # First argument is a number (group size)
            group_size = args[0]  # Extract group size
            names = args[1:]  # Remaining args are names (if any)
        else:  # First argument is a name (no group size provided)
            group_size = None  # Use default group size in C program
            names = args  # Treat all arguments as names

    # Run the C program with extracted or default arguments
    output = run_c_program(channel_id, BOT_TOKEN, group_size, names)

    # Send the message to the whole channel
    client.chat_postMessage(channel=channel_id, text="<!channel>")
    formatted_message = f"👥 *이번 주 랜덤밥🍚:*\n{output}"
    client.chat_postMessage(channel=channel_id, text=formatted_message)


# Start Slack Bot in Socket Mode
def main():
    handler = SocketModeHandler(app, APP_TOKEN)
    handler.start()


if __name__ == "__main__":
    main()
