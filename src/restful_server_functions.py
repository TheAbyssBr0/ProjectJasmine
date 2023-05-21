import json
from flask import Flask, request, jsonify, g
import database
from time_24h import get_date, get_time

app = Flask(__name__)


@app.before_request
def before_request():
    database_filename: str = "test.db"
    g.db = database.get_database_connection(database_filename)


@app.teardown_request
def teardown_request(exception):
    if hasattr(g, 'db'):
        g.db.close()


@app.route('/api/super_task', methods=['POST'])
def add_super_task():
    data_json = request.get_json()

    if not data_json or not data_json.get('title'):
        return jsonify({'message': 'Invalid data for super task'}), 400

    database.create_super_task(g.db, data_json['title'])
    return jsonify({'message': 'Created supertask ' + data_json['title']}), 201


@app.route('/api/super_task', methods=['GET'])
def get_super_task():
    return json.dumps(database.read_super_tasks(g.db), default=lambda x: x.__dict__), 201


@app.route('/api/super_task/<int:id>', methods=['PUT'])
def update_super_task(id):
    data_json = request.get_json()

    if not data_json or not data_json.get('title'):
        return jsonify({'message': 'Title must be provided for super task'}), 400

    database.update_super_task_title(g.db, id, data_json['title'])
    return jsonify({'message': 'Updated supertask, now named: ' + data_json['title']}), 201


@app.route('/api/super_task/<int:id>', methods=['DELETE'])
def delete_super_task(id):
    database.delete_super_task(g.db, id)
    return jsonify({'message': f'Supertask with id:{id} has been deleted.'}), 201


@app.route('/api/task', methods=['POST'])
def add_task():
    data_json = request.get_json()

    if not data_json or not data_json.get('title') or not data_json.get('super_task_id'):
        return jsonify({'message': 'Invalid data for task'}), 400

    database.create_task(g.db, data_json['super_task_id'], data_json['title'])
    return jsonify({'message': 'Created task ' + data_json['title']}), 201


@app.route('/api/task/<int:id>', methods=['PUT'])
def update_task(id):
    data_json = request.get_json()

    if not data_json or not data_json.get('title'):
        return jsonify({'message': 'Title must be provided for task'}), 400

    database.update_task_title(g.db, id, data_json['title'])
    return jsonify({'message': 'Updated task, now named: ' + data_json['title']}), 201


@app.route('/api/task/<int:id>', methods=['DELETE'])
def delete_task(id):
    database.delete_task(g.db, id)
    return jsonify({'message': f'Deleted task with id {id}'})


@app.route('/api/task/checkmark/<int:id>', methods=['PUT'])
def update_toggle_task_checkmark(id):
    return jsonify({'message': 'Changed checkmark to ' + str(database.update_task_checkmark_toggle(g.db, id))}), 201


@app.route('/api/ongoing_task', methods=['POST'])
def add_ongoing_task():
    data = request.get_json()

    if not data or not data.get('task_id'):
        return jsonify({'message': 'Invalid data for ongoing task'}), 400

    database.create_ongoing_task(g.db, data['task_id'])
    return jsonify({'message': 'Added task id: ' + str(data['task_id']) + ' as ongoing'}), 201


@app.route('/api/ongoing_task/<int:date>', methods=['GET'])
def get_ongoing_task_from_date(date):
    return json.dumps(database.read_all_tasks_from_date(g.db, date), default=lambda x: x.__dict__), 201


@app.route('/api/ongoing_task', methods=['PUT'])
def update_ongoing_task():
    data_json = request.get_json()
    if not data_json or not data_json.get('task_id') or not data_json.get('old_start_time'):
        return jsonify({'message': 'Not enough arguments provided.'}), 400

    date = get_date() if not data_json.get('date') else data_json['date']
    new_start_time = None if not data_json.get('new_start_time') else data_json['new_start_time']
    new_end_time = get_time() if not data_json.get('new_end_time') else data_json['new_end_time']

    database.update_ongoing_task(g.db, data_json['task_id'], data_json['old_start_time'], date, new_start_time,
                                 new_end_time)
    return jsonify({'message': 'Updated ongoing task id: ' + str(data_json['task_id'])})


@app.route('/api/ongoing_task/', methods=['DELETE'])
def delete_ongoing_task():
    data_json = request.get_json()

    if not data_json or not data_json.get('task_id') or not data_json.get('start_time') or not data_json.get('date'):
        return jsonify({'message': 'Invalid data for ongoing task'}), 400

    database.delete_ongoing_task(g.db, data_json['task_id'], data_json['start_time'], data_json['date'])
    return jsonify({'message': 'Deleted ongoing task id: ' + str(data_json['task_id'])})


def run_rest_api_server():
    app.run(debug=True)


if __name__ == "__main__":
    run_rest_api_server()
