<?php

namespace App\Http\Controllers;

use App\Events\RFIDTagReadEvent;
use Illuminate\Http\Request;

class RFIDTagController extends Controller
{
    /**
     * Display a listing of the resource.
     */
    public function index()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     */
    public function store(Request $request)
    {
        $request->validate([
            'tag_id' => 'required|string',
        ]);

        RFIDTagReadEvent::dispatch($request->tag_id);

        return response()->json([
            'message' => 'RFID tag read event dispatched.',
        ]);
    }

    /**
     * Display the specified resource.
     */
    public function show(string $id)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(Request $request, string $id)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     */
    public function destroy(string $id)
    {
        //
    }
}
