#include "Model.h"

Model::Model()
	:vertexBuffer_(nullptr),
	indexBuffer_(nullptr)
{}

Model::~Model() {
	if (vertexBuffer_) vertexBuffer_->Release();
	if (indexBuffer_) indexBuffer_->Release();
	vertexBuffer_ = nullptr;
	indexBuffer_ = nullptr;
}