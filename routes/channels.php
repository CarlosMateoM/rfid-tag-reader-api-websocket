<?php

use App\Broadcasting\RFIDTagReadChannel;
use Illuminate\Support\Facades\Broadcast;

Broadcast::channel('App.Models.User.{id}', function ($user, $id) {
    return (int) $user->id === (int) $id;
});


Broadcast::channel('rfid-tag-read', RFIDTagReadChannel::class);